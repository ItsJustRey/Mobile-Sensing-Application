#include "systemc.h"
#include <vector>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

const int NUM_ROI = 5;

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
	
	sc_signal<bool> ROI_INDEX[NUM_ROI];
	double ROI_TIME_START[NUM_ROI];
	double ROI_TIME_END[NUM_ROI];

	void prc_mobile();
	void prc_generate_index();

	SC_HAS_PROCESS(MOBILE);
	MOBILE(sc_module_name name) :
		sc_module(name)
	{

		cout << "CREATING MOBILE..." << "\tName: " << name << endl;
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


		SC_METHOD(prc_generate_index);
		for (int i = 0; i < NUM_ROI; i++){
			sensitive << ROI_INDEX[i].value_changed_event();
		}
		dont_initialize();

	}
private:


};