#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
const int NUM_MOBILES = 3;
const int MAX_TUPPLE_SIZE = 20;
const int SERVER_ARRAY_NUM_COLUMNS = 5;

template <class T> class SERVER : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;
	sc_out<bool> free_out[NUM_MOBILES];						// SERVER[i] --> MOBILE[i]
	sc_in<bool> packet_request_in[NUM_MOBILES];				// SERVER[i] <-- MOBILE[i]
	sc_out<bool> packet_permission_out[NUM_MOBILES];		// SERVER[i] --> MOBILE[i]
	sc_in<bool> start_transmission_in[NUM_MOBILES];			// SERVER[i] <-- MOBILE[i]

	
	sc_signal<bool> transmitting;
	bool is_transmitting;
	bool server_is_free;

	sc_int<8>  server_array[NUM_MOBILES][SERVER_ARRAY_NUM_COLUMNS];	// SERVER DATA STRUCTURE 



	void prc_receive_from_mobile();
	void prc_start_transmission();
	void prc_transmit();
	void print_server();

	SC_HAS_PROCESS(SERVER);
	SERVER(sc_module_name name) :
		sc_module(name)

	{
		is_transmitting = false;
		server_is_free = true;
		cout << "CREATING SERVER..." << "\tName: " << name << endl;

		for (int i = 0; i < NUM_MOBILES; i++)
		{
			free_out[i].initialize(1);
		}

		SC_METHOD(prc_receive_from_mobile);
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			sensitive << packet_request_in[i].pos();
		}
		dont_initialize();

		SC_THREAD(prc_transmit);
		sensitive << transmitting.posedge_event();
		dont_initialize();

		SC_METHOD(print_server);
		sensitive << clock.pos();
		dont_initialize();
	}
private:

};