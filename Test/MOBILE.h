#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int NUM_ROI = 5;
const int tuple_NUM_COLUMNS = 3;
const int PACKET_SIZE = 10;
const int NUM_IMAGES = 2;
const int IMAGE_INDEX_0 = 0;
const int IMAGE_INDEX_1 = 1;

template <class T> class MOBILE : public sc_module{
public:

	//PORTS
	sc_in<bool> clock;
	sc_in<sc_int<16> > randX;
	sc_in<sc_int<16> > randY;

	sc_in<bool> free_in;						// SERVER --> MOBILE[i]
	sc_signal<bool> packet_full;
	sc_out<bool> packet_request_out;				// SERVER[i] <-- MOBILE[i]
	sc_in<bool> packet_permission_in;			// SERVER[i] --> MOBILE[i]
	sc_out<bool> start_transmission_out;	// SERVER[i] <-- MOBILE[i]
	sc_in<bool> done_in;						// SERVER[i] --> MOBILE[i]

	

	sc_int<16>  LEFT_BOTTOM_X[NUM_ROI][NUM_IMAGES];
	sc_int<16>  LEFT_BOTTOM_Y[NUM_ROI][NUM_IMAGES];
	sc_int<16>  RIGHT_TOP_X[NUM_ROI][NUM_IMAGES];
	sc_int<16>  RIGHT_TOP_Y[NUM_ROI][NUM_IMAGES];
	
	sc_signal<bool> ROI_INDEX_SIG[NUM_ROI];					// SIGNAL THAT INDICATES IF X AND Y ARE WITHIN AN ROI
	
	double  TUPLE_ARRAY[PACKET_SIZE][tuple_NUM_COLUMNS];	// TUPLE DATA STRUCTURE
	double	ROI_TIME_START[NUM_ROI];							// ARRAY TO HOLD EACH TUPLE's START TIME
	double	ROI_TIME_END[NUM_ROI];							// ARRAY TO HOLD EACH TUPLE's END TIME

	int tuple_counter;										// NUMBER OF TUPLES
	int packet_counter;										// NUMBER OF PACKETS
	bool transmitting;


	void prc_mobile();
	void detect_tuple();	
	void prc_request_to_server();
	void transmission_done();
	void print_mobile();

	SC_HAS_PROCESS(MOBILE);
	MOBILE(sc_module_name name, const T* mobile_id) :
		sc_module(name), _mobile_id(mobile_id)
	{

		cout << "CREATING MOBILE..." << "\tName: " << name << endl;

		tuple_counter = 0;
		packet_counter = 0;
		transmitting = false;
		for (int i = 0; i < NUM_IMAGES; i++)
		{
			LEFT_BOTTOM_X[0][i] = 50;
			LEFT_BOTTOM_X[1][i] = 50;
			LEFT_BOTTOM_X[2][i] = 470;
			LEFT_BOTTOM_X[3][i] = 670;
			LEFT_BOTTOM_X[4][i] = 680;

			LEFT_BOTTOM_Y[0][i] = 20;
			LEFT_BOTTOM_Y[1][i] = 370;
			LEFT_BOTTOM_Y[2][i] = 20;
			LEFT_BOTTOM_Y[3][i] = 40;
			LEFT_BOTTOM_Y[4][i] = 700;

			RIGHT_TOP_X[0][i] = 400;
			RIGHT_TOP_X[1][i] = 450;
			RIGHT_TOP_X[2][i] = 600;
			RIGHT_TOP_X[3][i] = 950;
			RIGHT_TOP_X[4][i] = 1000;

			RIGHT_TOP_Y[0][i] = 320;
			RIGHT_TOP_Y[1][i] = 1000;
			RIGHT_TOP_Y[2][i] = 900;
			RIGHT_TOP_Y[3][i] = 550;
			RIGHT_TOP_Y[4][i] = 1000;
		}
		
		SC_METHOD(prc_mobile);
		sensitive << clock.pos();
		dont_initialize();

		SC_METHOD(detect_tuple);
		for (int i = 0; i < NUM_ROI; i++){
			sensitive << ROI_INDEX_SIG[i].value_changed_event();
		}
		dont_initialize();

		SC_THREAD(prc_request_to_server);
		sensitive << packet_full.posedge_event();
		dont_initialize();

		/*SC_METHOD(transmission_done);
		sensitive << done_in.pos();
		dont_initialize();*/

		SC_METHOD(print_mobile);
		sensitive << clock.pos();
		dont_initialize();

	}
private:
	const T* _mobile_id;

};