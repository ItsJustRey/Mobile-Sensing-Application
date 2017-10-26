#include "SENSOR.h"
//

typedef int sensor_T;
void SENSOR<sensor_T>::prc_sensor(){

	cout << endl;
	cout << "prc_sensor() called at " << sc_time_stamp().to_seconds() << endl;
}