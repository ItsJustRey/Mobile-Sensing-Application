#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;


template <class T> class SENSOR : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;

	sc_out<sc_uint<8> > randX;
	sc_out<sc_uint<8> > randY;

	void prc_sensor();

	SC_HAS_PROCESS(SENSOR);
	SENSOR(sc_module_name name) :
		sc_module(name)
	{

		cout << "CREATING SENSOR..." << "\tName: " << name << endl;

		SC_METHOD(prc_sensor);
		sensitive << clock.pos();
		dont_initialize();

	}
private:


};