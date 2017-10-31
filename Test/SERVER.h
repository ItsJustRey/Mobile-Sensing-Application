#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;
const int NUM_MOBILES = 3;


template <class T> class SERVER : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;
	sc_in<bool> packet_signal[NUM_MOBILES];

	void prc_server();
	SC_HAS_PROCESS(SERVER);
	SERVER(sc_module_name name) :
		sc_module(name)

	{

		cout << "CREATING SERVER..." << "\tName: " << name << endl;

		SC_METHOD(prc_server);
		sensitive << clock.pos();

	}
private:

};