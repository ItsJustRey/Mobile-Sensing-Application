#include "SENSOR.h"
#include <stdlib.h>    
#include <time.h>

typedef int sensor_T;

void SENSOR<sensor_T>::prc_sensor(){

	cout << endl;
	cout << endl;
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "prc_sensor"<<*(_sensor_id)<< "() called at " << sc_time_stamp().to_seconds() << endl;
	//srand(time(NULL));
	int x = rand() % 1023;
	int y = rand() % 1023;

	randX.write(x);
	randY.write(y);

	cout << "SENSOR " <<*(_sensor_id)<<" HAS WRITTEN X = " << randX.read() << " Y = " << randY.read() << endl;

}