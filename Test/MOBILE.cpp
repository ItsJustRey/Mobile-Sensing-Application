#include "MOBILE.h"

typedef int mobile_T;

void MOBILE<mobile_T>::prc_mobile(){

	cout << "prc_mobile() called at " << sc_time_stamp().to_seconds() << endl;
	//cout << "MOBILE "<<*(_mobile_id)<<" HAS READ X = " << randX.read() <<  " Y = " << randY.read() << endl;

	for (int i = 0; i < NUM_ROI; i++)
	{

		//if(imageIndex.read()==0)
		// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
		if (((LEFT_BOTTOM_X[i][IMAGE_INDEX_0] <= randX.read()) && (randX.read() <= RIGHT_TOP_X[i][IMAGE_INDEX_0])) &&
			((LEFT_BOTTOM_Y[i][IMAGE_INDEX_0] <= randY.read()) && (randY.read() <= RIGHT_TOP_Y[i][IMAGE_INDEX_0]))
		   )
		{
			cout << "X AND Y WITHIN ROI " << i+1 << endl;
			//cout << "WRITING A POSEDGE TO ROI" << i + 1 << endl;
			ROI_INDEX_SIG[i].write(true);
		}
		else {
			//cout << "WRITING A NEGEDGE TO ROI" << i + 1 << endl;
			ROI_INDEX_SIG[i].write(false);
		}

		//else if(imageIndex.read()==1)
		//// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
		//if (((LEFT_BOTTOM_X[i][IMAGE_INDEX+1] <= randX.read()) && (randX.read() <= RIGHT_TOP_X[i][IMAGE_INDEX+1])) &&
		//	((LEFT_BOTTOM_Y[i][IMAGE_INDEX+1] <= randY.read()) && (randY.read() <= RIGHT_TOP_Y[i][IMAGE_INDEX+1]))
		//	)
		//{
		//	cout << "X AND Y WITHIN ROI " << i + 1 << endl;
		//	cout << "WRITING A POSEDGE TO ROI" << i + 1 << endl;
		//	ROI_INDEX_SIG[i].write(true);
		//}
		//else {
		//	cout << "WRITING A NEGEDGE TO ROI" << i + 1 << endl;
		//	ROI_INDEX_SIG[i].write(false);
		//}

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
			//cout << "DETECTED POSEDGE TO ROI" << i + 1 << endl;
			ROI_TIME_START[i] = sc_time_stamp().to_seconds();

		}
		// END TIME
		if (ROI_INDEX_SIG[i].negedge()){
			//cout << "DETECTED NEGEDGE TO ROI" << i + 1 << endl;
			ROI_TIME_END[i] = sc_time_stamp().to_seconds();

			// UPDATE tuple ARRAY
			if (tuple_counter <= PACKET_SIZE - 1)
			{
				TUPLE_ARRAY[tuple_counter][0] = i + 1;
				TUPLE_ARRAY[tuple_counter][1] = ROI_TIME_START[i];
				TUPLE_ARRAY[tuple_counter][2] = ROI_TIME_END[i];
				tuple_counter++;
				cout <<"MOBILE " << *(_mobile_id) <<  " Tuple COUNT " << tuple_counter << endl;
			}
			else
			{
				cout << "MOBILE " << *(_mobile_id) << " PACKET IS FULL" << endl;
				packet_full.write(1);
			}
		}
	}
}


// SC_THREAD triggered on packet_full
void MOBILE<mobile_T>::prc_request_to_server(){
	while (1)
	{
		cout << "MOBILE " << *(_mobile_id) << " REQUEST TO SERVER" << endl;
		// SERVER IS FREE AND PACKET IS FULL
		if (free_in.read() == 1)
		{
			cout << "SERVER IS FREE" << endl;
			packet_request.write(1);
			if (packet_permission_in.read() == 1)
			{
				cout << "MOBILE " << *(_mobile_id) << " STARTING TRANSMISSION!" << endl;
				start_transmission_out.write(1);
				wait(8, SC_MS);
				//
				
			}
			else
			{
				//cout << "PACKET PERMISSION NOT GRANTED to MOBILE " << *(_mobile_id) << endl;
				wait(8, SC_MS);
			}
		}
		else{
			wait(8, SC_MS);
		}
	}
}


// SC_METHOD triggered on done_in
void MOBILE<mobile_T>::transmission_done(){

	if (done_in.read() == 1)
	{
		packet_counter++;
		cout << "PACKET COUNT " << packet_counter << endl;



	}
	else
	{



	}




}



void MOBILE<mobile_T>::print_tuple_data(){

	/*cout << endl << "======Tuple ARRAY in Mobile " << *(_mobile_id) << "=======" << endl;
	cout << "|ROI\t|START\t|END\t|" << endl;
	cout << "==============================" << endl;
	for (int i = 0; i < PACKET_SIZE; i++){

		cout << "|";
		for (int j = 0; j < tuple_NUM_COLUMNS; j++){
			cout << TUPLE_ARRAY[i][j] << "\t|";
		}
		cout << endl;
	}
	cout << "=========================================" << endl;*/
}
