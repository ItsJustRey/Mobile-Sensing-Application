#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int NUM_ROI = 5;
const int tuple_NUM_COLUMNS = 3;
const int PACKET_SIZE = 20;
const int IMAGE_INDEX=0;
const int IMAGES = 2;

template <class T> class MOBILE : public sc_module{
public:

	//PORTS
	sc_in<bool> clock;

	sc_in<sc_int<16> > randX;
	sc_in<sc_int<16> > randY;


	sc_int<16>  LEFT_BOTTOM_X[NUM_ROI][IMAGES];
	sc_int<16>  LEFT_BOTTOM_Y[NUM_ROI][IMAGES];
	sc_int<16>  RIGHT_TOP_X[NUM_ROI][IMAGES];
	sc_int<16>  RIGHT_TOP_Y[NUM_ROI][IMAGES];
	
	sc_signal<bool> ROI_INDEX_SIG[NUM_ROI];					// SIGNAL THAT INDICATES IF X AND Y ARE WITHIN AN ROI
	sc_out<bool>packet_signal;
	



	double ROI_TIME_START[NUM_ROI];							// ARRAY TO HOLD EACH TUPLE's START TIME
	double ROI_TIME_END[NUM_ROI];							// ARRAY TO HOLD EACH TUPLE's END TIME

	double  ROI_INDEX_ARRAY[NUM_ROI];						// COLUMN FOR EACH tuple's ROI
	double  ROI_START_TIME_ARRAY[NUM_ROI];					// COLUMN FOR EACH TUPLE's START TIME
	double  ROI_END_TIME_ARRAY[NUM_ROI];					// COLUMN FOR EACH TUPLE's END TIME
	double  TUPLE_ARRAY[PACKET_SIZE][tuple_NUM_COLUMNS];	// TUPLE DATA STRUCTURE

	int tuple_count;										// NUMBER OF TUPLES


	void prc_mobile();
	void detect_tuple();
	void print_tuple_data();
	void write_to_server();


	SC_HAS_PROCESS(MOBILE);
	MOBILE(sc_module_name name, const T* mobile_id) :
		sc_module(name), _mobile_id(mobile_id)
	{

		cout << "CREATING MOBILE..." << "\tName: " << name << endl;

		tuple_count = 0;

		LEFT_BOTTOM_X[0][IMAGE_INDEX] = 50;
		LEFT_BOTTOM_X[1][IMAGE_INDEX] = 50;
		LEFT_BOTTOM_X[2][IMAGE_INDEX] = 470;
		LEFT_BOTTOM_X[3][IMAGE_INDEX] = 670;
		LEFT_BOTTOM_X[4][IMAGE_INDEX] = 680;

		LEFT_BOTTOM_Y[0][IMAGE_INDEX] = 20;
		LEFT_BOTTOM_Y[1][IMAGE_INDEX] = 370;
		LEFT_BOTTOM_Y[2][IMAGE_INDEX] = 20;
		LEFT_BOTTOM_Y[3][IMAGE_INDEX] = 40;
		LEFT_BOTTOM_Y[4][IMAGE_INDEX] = 700;

		RIGHT_TOP_X[0][IMAGE_INDEX] = 400;
		RIGHT_TOP_X[1][IMAGE_INDEX] = 450;
		RIGHT_TOP_X[2][IMAGE_INDEX] = 600;
		RIGHT_TOP_X[3][IMAGE_INDEX] = 950;
		RIGHT_TOP_X[4][IMAGE_INDEX] = 1000;

		RIGHT_TOP_Y[0][IMAGE_INDEX] = 320;
		RIGHT_TOP_Y[1][IMAGE_INDEX] = 1000;
		RIGHT_TOP_Y[2][IMAGE_INDEX] = 900;
		RIGHT_TOP_Y[3][IMAGE_INDEX] = 550;
		RIGHT_TOP_Y[4][IMAGE_INDEX] = 1000;

		SC_METHOD(prc_mobile);
		sensitive << clock.pos();
		dont_initialize();

		SC_METHOD(detect_tuple);
		for (int i = 0; i < NUM_ROI; i++){
			sensitive << ROI_INDEX_SIG[i].value_changed_event();
		}
		dont_initialize();

		SC_METHOD(print_tuple_data);
		sensitive << clock.pos();
		dont_initialize();

		SC_METHOD(write_to_server);
		sensitive << packet_signal.pos();
		dont_initialize();
	}
private:
	const T* _mobile_id;

};