// All systemc modules should include systemc.h header file
#include "systemc.h"
#include "SENSOR.h"
#include "MOBILE.h"
#include "SERVER.h"
#define _CRT_SECURE_NO_WARNINGS
const int NUM_MOBILE = 3;

int sc_main(int argc, char *argv[]){

	// DEFINE SIGNALS
	sc_clock					clk_sig("clk", 4, SC_MS);

	sc_signal<sc_int<16> >	 randX_sig[NUM_MOBILE];
	sc_signal<sc_int<16> >	 randY_sig[NUM_MOBILE];
	sc_signal<bool>			 pack_sig[NUM_MOBILE];



	
	// CREATE SENSOR 0
	typedef int sensor0_T;
	const sensor0_T sensor0_id = 0;
	SENSOR<sensor0_T>	sensor0("sensor0", &sensor0_id);
	sensor0.clock(clk_sig);
	sensor0.randX(randX_sig[0]);
	sensor0.randY(randY_sig[0]);

	// CREATE SENSOR 1
	typedef int sensor1_T;
	const sensor1_T sensor1_id = 1;
	SENSOR<sensor1_T>	sensor1("sensor1", &sensor1_id);
	sensor1.clock(clk_sig);
	sensor1.randX(randX_sig[1]);	
	sensor1.randY(randY_sig[1]);

	// CREATE SENSOR 2
	typedef int sensor2_T;
	const sensor2_T sensor2_id = 2;
	SENSOR<sensor2_T>	sensor2("sensor2",&sensor2_id);
	sensor2.clock(clk_sig);
	sensor2.randX(randX_sig[2]);
	sensor2.randY(randY_sig[2]);

	// CREATE MOBILE 0
	typedef int mobile0_T;
	const mobile0_T mobile0_id = 0;
	MOBILE<mobile0_T>	mobile0("mobile0",&mobile0_id);
	mobile0.clock(clk_sig);
	mobile0.randX(randX_sig[0]);
	mobile0.randY(randY_sig[0]);
	mobile0.packet_signal(pack_sig[0]);

	// CREATE MOBILE 1
	typedef int mobile1_T;
	const mobile1_T mobile1_id = 1;
	MOBILE<mobile1_T>	mobile1("mobile1", &mobile1_id);
	mobile1.clock(clk_sig);
	mobile1.randX(randX_sig[1]);
	mobile1.randY(randY_sig[1]);
	mobile1.packet_signal(pack_sig[1]);

	// CREATE MOBILE 2
	typedef int mobile2_T;
	const mobile2_T mobile2_id = 2;
	MOBILE<mobile2_T>	mobile2("mobile2", &mobile2_id);
	mobile2.clock(clk_sig);
	mobile2.randX(randX_sig[2]);
	mobile2.randY(randY_sig[2]);
	mobile2.packet_signal(pack_sig[2]);

	//CREATE SERVER
	typedef int server_T;
	SERVER <server_T> server("server");
	server.clock(clk_sig);
	for (int i = 0; i < NUM_MOBILE; i++)
	{
		server.packet_signal[i](pack_sig[i]);

	}


	// CONTUINUE FOR 1 SECOND
	sc_start(.5, SC_SEC);

	return 0;
}