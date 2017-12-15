#include "systemc.h"
#include "CONSTANTS.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;


//const int NUM_ROI = 5;
//const int NUM_IMAGES = 5;
//
//const int tuple_NUM_COLUMNS = 3;
//const int PACKET_SIZE = 20;
//
//const int NUM_MOBILES = 3;
//const int MAX_TUPPLE_SIZE = 20;
//const int SERVER_ARRAY_NUM_COLUMNS = 5;
//const int BANDWIDTH_512KBS				=  524288;
//const int BANDWIDTH_1MBS				= 1048576;
//const int BANDWIDTH_5MBS				=  5242880;
//const int BANDWIDTH_10MBS				= 10485760;
//const int SERVER_TO_MOBILE_PACKET_1MB	=  1048576;
//const int SERVER_TO_MOBILE_PACKET_10MB	= 10485760;
//const int IMAGE_SIZE					=  8388608;
//
//
//// TUPLE SIZE
//const int TUPLE_SIZE = 192;	// 3 * 64
//
//// RECEIVE PACKET SIZE
//const int TRANSMIT_PACKET_SIZE = 3840;	// 20 * 3 * 64
//const double IMAGE1_TIME = 100.00;
//const double IMAGE2_TIME = 120.00;
//const double IMAGE3_TIME = 125.00;
//const double IMAGE4_TIME = 130.00;
//const double IMAGE5_TIME = 150.00;

template <class T> class SERVER : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;
	sc_out<bool> free_out[NUM_MOBILES];						// SERVER --> MOBILE[i]
	sc_in<bool> packet_request_in[NUM_MOBILES];				// SERVER <-- MOBILE[i]
	sc_out<bool> packet_permission_out[NUM_MOBILES];		// SERVER --> MOBILE[i]
	sc_in<bool> start_transmission_in[NUM_MOBILES];			// SERVER <-- MOBILE[i]

	sc_out<sc_int<16> > send_new_image_out[NUM_MOBILES];	// SERVER --> MOBILE[i]
	sc_out<bool> image_transmitted_done_out[NUM_MOBILES];		// SERVER --> MOBILE[i]



	sc_signal<bool> transmitting[NUM_MOBILES];
	sc_signal<bool> image_transmitted_done_sig[NUM_MOBILES];
	bool is_transmitting;
	bool server_is_free;

	sc_int<8>  server_array[NUM_MOBILES][SERVER_ARRAY_NUM_COLUMNS];	// SERVER DATA STRUCTURE 

	int currentImageIndex;

	double transmission_time;									//transmission time = PACKET_SIZE / BANDWIDTH
	//int iterations_per_image = IMAGE_SIZE / BANDWIDTH_1MBS;	//iterations = IMAGE_SIZE / BANDWIDTH
	double image_bits_transmitted[NUM_MOBILES];
	double bandwidth;
	double packet_size;
	void send_new_image();
	//void image_transmitted_done();
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

		bandwidth = BANDWIDTH_1MBS;
		packet_size = SERVER_TO_MOBILE_PACKET_1MB;
		transmission_time = packet_size / bandwidth;
		

		SC_METHOD(send_new_image);
		sensitive << clock.pos();

		
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			free_out[i].initialize(1);
			image_bits_transmitted[i] = 0;
		}

		SC_METHOD(prc_receive_from_mobile);
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			sensitive << packet_request_in[i].pos();
		}
		dont_initialize();

		SC_THREAD(prc_transmit);
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			sensitive << transmitting[i].posedge_event();
		}
		
		dont_initialize();
/*
		SC_METHOD(image_transmitted_done);
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			sensitive << image_transmitted_done_sig[i].posedge_event();
		}

		dont_initialize();*/

		SC_METHOD(print_server);
		sensitive << clock.pos();
		dont_initialize();
	}
private:

};