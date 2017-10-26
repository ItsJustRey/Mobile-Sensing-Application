// All systemc modules should include systemc.h header file
#include "systemc.h"
#include "SENSOR.h"
#include "MOBILE.h"

#define _CRT_SECURE_NO_WARNINGS
int sc_main(int argc, char *argv[]){

	// DEFINE SIGNALS
	sc_clock					clk_sig("clk", 4, SC_MS);

	// CREATE SENSOR 0
	typedef int sensor_T;

	SENSOR<sensor_T>	sensor0("sensor0");
	sensor0.clock(clk_sig);



	// CREATE MOBILE0
	typedef int mobile_T;
	MOBILE<mobile_T>	mobile0("mobile0");
	mobile0.clock(clk_sig);


	// CONTUINUE FOR 1 SECOND
	sc_start(1, SC_SEC);


	return 0;
}