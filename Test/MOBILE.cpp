#include "MOBILE.h"

typedef int mobile_T;
void MOBILE<mobile_T>::prc_mobile(){

	cout << "prc_mobile() called at " << sc_time_stamp().to_seconds() << endl;

	cout << "MOBILE HAS READ X = " << randX.read() <<  " Y = " << randY.read() << endl;

	for (int i = 0; i < NUM_ROI; i++)
	{

		// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
		if ( ( (LEFT_BOTTOM_X[i] <= randX.read() ) && (randX.read() <= RIGHT_TOP_X[i]) ) && ( (LEFT_BOTTOM_Y[i] <= randY.read() ) && ( randY.read() <= RIGHT_TOP_Y[i] ) ) )
		{
			cout << "X AND Y WITHIN ROI " << i+1 << endl;


			ROI_INDEX[i].write(true);

			break;

		}
		else{
			ROI_INDEX[i].write(false);
		}


	}

	cout << endl;
	cout << endl;

}

void MOBILE<mobile_T>::prc_generate_index(){

	for (int i = 0; i < NUM_ROI; i++)
	{

		// START TIME
		if (ROI_INDEX[i].posedge()){
			ROI_TIME_START[i] = sc_time_stamp().to_seconds();
		}
		else if (ROI_INDEX[i].negedge()){
			ROI_TIME_END[i] = sc_time_stamp().to_seconds();
		}


	}


	// OUTPUT PACKETS
	for (int i = 0; i < NUM_ROI; i++)
	{
		cout << "ROI " << i+1 << " start time: " << ROI_TIME_START[i] << " end time: " << ROI_TIME_END[i] << endl;

	}
}