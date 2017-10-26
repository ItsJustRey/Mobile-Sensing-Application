#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
using namespace std;




template <class T> class MOBILE : public sc_module{
public:

	//	PORTS
	sc_in<bool> clock;

	void prc_mobile();

	SC_HAS_PROCESS(MOBILE);
	MOBILE(sc_module_name name) :
		sc_module(name)
	{

		cout << "CREATING MOBILE..." << "\tName: " << name << endl;




		SC_METHOD(prc_mobile);
		sensitive << clock.pos();
		dont_initialize();

	}
private:


};