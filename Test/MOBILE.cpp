#include "MOBILE.h"

typedef int mobile_T;

void MOBILE<mobile_T>::prc_mobile(){

	cout << "prc_mobile() called at " << sc_time_stamp().to_seconds() << endl;
	cout << "MOBILE HAS READ X = " << randX.read() <<  " Y = " << randY.read() << endl;

	for (int i = 0; i < NUM_ROI; i++)
	{

		// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
		if ( ( (LEFT_BOTTOM_X[i] <= randX.read() ) && (randX.read() <= RIGHT_TOP_X[i]) ) && 
			 ( (LEFT_BOTTOM_Y[i] <= randY.read() ) && (randY.read() <= RIGHT_TOP_Y[i]) ) 
		   )
		{
			cout << "X AND Y WITHIN ROI " << i+1 << endl;
			cout << "WRITING A POSEDGE TO ROI" << i + 1 << endl;
			ROI_INDEX_SIG[i].write(true);
		}
		else{
			cout << "WRITING A NEGEDGE TO ROI" << i + 1 << endl;
			ROI_INDEX_SIG[i].write(false);
		}
	}

	cout << endl;
	cout << endl;
}

void MOBILE<mobile_T>::detect_tuple(){

	// ITERATE THROUGH ALL ROI's
	for (int i = 0; i < NUM_ROI; i++)
	{

		// START TIME
		if (ROI_INDEX_SIG[i].posedge()){
			cout << "DETECTED POSEDGE TO ROI" << i + 1 << endl;
			ROI_TIME_START[i] = sc_time_stamp().to_seconds();

		}
		// END TIME
		if (ROI_INDEX_SIG[i].negedge()){
			cout << "DETECTED NEGEDGE TO ROI" << i + 1 << endl;
			ROI_TIME_END[i] = sc_time_stamp().to_seconds();

			// UPDATE tuple ARRAY
			TUPLE_ARRAY[tuple_count][0] = i + 1;
			TUPLE_ARRAY[tuple_count][1] = ROI_TIME_START[i];
			TUPLE_ARRAY[tuple_count][2] = ROI_TIME_END[i];
			tuple_count++;
			cout << "tuple COUNT " << tuple_count << endl;
		}
	}
}

void MOBILE<mobile_T>::print_tuple_data(){

	cout << endl << "======tuple ARRAY=======" << endl;
	cout << "|ROI\t|START\t|END\t|" << endl;
	cout << "==============================" << endl;
	for (int i = 0; i < PACKET_SIZE; i++){

		cout << "|";
		for (int j = 0; j < tuple_NUM_COLUMNS; j++){
			cout << TUPLE_ARRAY[i][j] << "\t|";
		}
		cout << endl;
	}
	cout << "=========================================" << endl;
}