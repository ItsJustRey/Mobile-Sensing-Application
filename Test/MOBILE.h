#include "systemc.h"
#include <vector>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int NUM_ROI = 5;
const int tuple_NUM_COLUMNS = 3;
const int PACKET_SIZE = 50;

template <class T> class MOBILE : public sc_module{
public:

	//PORTS
	sc_in<bool> clock;

	sc_in<sc_int<16> > randX;
	sc_in<sc_int<16> > randY;

	sc_int<16>  COUNT;
	sc_int<16>  LEFT_BOTTOM_X[NUM_ROI];
	sc_int<16>  LEFT_BOTTOM_Y[NUM_ROI];
	sc_int<16>  RIGHT_TOP_X[NUM_ROI];
	sc_int<16>  RIGHT_TOP_Y[NUM_ROI];
	
	sc_signal<bool> ROI_INDEX_SIG[NUM_ROI];					// SIGNAL THAT INDICATES IF X AND Y ARE WITHIN AN ROI		
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


	SC_HAS_PROCESS(MOBILE);
	MOBILE(sc_module_name name) :
		sc_module(name)
	{

		cout << "CREATING MOBILE..." << "\tName: " << name << endl;

		tuple_count = 0;

		LEFT_BOTTOM_X[0] = 50;
		LEFT_BOTTOM_X[1] = 50;
		LEFT_BOTTOM_X[2] = 470;
		LEFT_BOTTOM_X[3] = 670;
		LEFT_BOTTOM_X[4] = 680;

		LEFT_BOTTOM_Y[0] = 20;
		LEFT_BOTTOM_Y[1] = 370;
		LEFT_BOTTOM_Y[2] = 20;
		LEFT_BOTTOM_Y[3] = 40;
		LEFT_BOTTOM_Y[4] = 700;

		RIGHT_TOP_X[0] = 400;
		RIGHT_TOP_X[1] = 450;
		RIGHT_TOP_X[2] = 600;
		RIGHT_TOP_X[3] = 950;
		RIGHT_TOP_X[4] = 1000;

		RIGHT_TOP_Y[0] = 320;
		RIGHT_TOP_Y[1] = 1000;
		RIGHT_TOP_Y[2] = 900;
		RIGHT_TOP_Y[3] = 550;
		RIGHT_TOP_Y[4] = 1000;

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
	}
private:


};