#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
const int NUM_MOBILES = 3;
const int MAX_TUPPLE_SIZE = 20;
const int TUPLE_BITS = 64;
const int SERVER_ARRAY_NUM_COLUMNS = 5;
const int BANDWIDTH = 1048576;
const int IMAGE_SIZE = 8000000;
const double IMAGE1_TIME = 100.00;
const double IMAGE2_TIME = 120.00;
const double IMAGE3_TIME = 125.00;
const double IMAGE4_TIME = 130.00;
const double IMAGE5_TIME = 150.00;

template <class T> class SERVER : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;
	sc_out<bool> free_out[NUM_MOBILES];						// SERVER --> MOBILE[i]
	sc_in<bool> packet_request_in[NUM_MOBILES];				// SERVER <-- MOBILE[i]
	sc_out<bool> packet_permission_out[NUM_MOBILES];		// SERVER --> MOBILE[i]
	sc_in<bool> start_transmission_in[NUM_MOBILES];			// SERVER <-- MOBILE[i]

	sc_out<sc_int<16> > send_new_image_out[NUM_MOBILES];	// SERVER --> MOBILE[i]

	sc_signal<bool> transmitting;
	bool is_transmitting;
	bool server_is_free;

	sc_int<8>  server_array[NUM_MOBILES][SERVER_ARRAY_NUM_COLUMNS];	// SERVER DATA STRUCTURE 

	int currentImageIndex;
	int receivepacketsize = MAX_TUPPLE_SIZE * NUM_MOBILES * TUPLE_BITS;
	int sendpacketsize = BANDWIDTH;

	double timeperimage = IMAGE_SIZE / BANDWIDTH;

	void send_new_image();
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


		SC_METHOD(send_new_image);
		sensitive << clock.pos();


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