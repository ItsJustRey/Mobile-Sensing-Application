#include "systemc.h"
#include "CONSTANTS.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;



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

		bandwidth = BANDWIDTH_5MBS;
		packet_size = SERVER_TO_MOBILE_PACKET_10MB;
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