#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

template <class T> class SENSOR : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;

	sc_out<sc_int<16> > randX;
	sc_out<sc_int<16> > randY;

	void prc_sensor();

	SC_HAS_PROCESS(SENSOR);
	SENSOR(sc_module_name name, const T* sensor_id) :
		sc_module(name), _sensor_id(sensor_id)

	{

		cout << "CREATING SENSOR..." << "\tName: " << name << endl;

		SC_METHOD(prc_sensor);
		sensitive << clock.pos();

	}
private:

	const T* _sensor_id;
};