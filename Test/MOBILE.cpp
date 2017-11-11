#include "MOBILE.h"

typedef int mobile_T;

void MOBILE<mobile_T>::prc_mobile(){

	
	
	if (!transmitting){
		cout << "MOBILE " << *(_mobile_id) << " HAS READ X = " << randX.read() << " Y = " << randY.read() << " at " << sc_time_stamp().to_seconds()  << endl;
		for (int i = 0; i < NUM_ROI; i++)
		{

			//if(imageIndex.read()==0)
			// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
			if (((LEFT_BOTTOM_X[i][IMAGE_INDEX_0] <= randX.read()) && (randX.read() <= RIGHT_TOP_X[i][IMAGE_INDEX_0])) &&
				((LEFT_BOTTOM_Y[i][IMAGE_INDEX_0] <= randY.read()) && (randY.read() <= RIGHT_TOP_Y[i][IMAGE_INDEX_0]))
				)
			{
				cout << "X AND Y WITHIN ROI " << i + 1 << endl;
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

	}
	cout << endl;
	cout << endl;
}



// sensitive to ROI_INDEX_SIG[i].value_changed_event();
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
				cout << "MOBILE " << *(_mobile_id) << " Tuple COUNT " << tuple_counter << " at " << sc_time_stamp().to_seconds() << endl;
			}
			else
			{
				cout << "MOBILE " << *(_mobile_id) << " PACKET IS FULL at " << sc_time_stamp().to_seconds() <<  endl;
				packet_full.write(1);
				
			}
		}
	}
}


// SC_THREAD triggered on packet_full
void MOBILE<mobile_T>::prc_request_to_server(){
	cout << "MOBILE " << *(_mobile_id) << " prc_request_to_server() triggered at " << sc_time_stamp().to_seconds() << endl;
	//while (!transmitting)
	while (true)
	{
		cout << " INSIDE MOBILE " << *(_mobile_id) << " prc_request_to_server() WHILE LOOP at " << sc_time_stamp().to_seconds() << endl;
		// SERVER IS FREE AND PACKET IS FULL
		if (free_in.read() == 1)
		{
			cout << "  MOBILE " << *(_mobile_id) << " SEES THAT SERVER IS FREE at " << sc_time_stamp().to_seconds() << endl;

			if (transmitting == false)
			{

				packet_request_out.write(1);
				cout << "   MOBILE " << *(_mobile_id) << " IS REQUESTING SERVER at " << sc_time_stamp().to_seconds() << endl;
				
			}
			
			if (packet_permission_in.read() == 1)
			{
				cout << "     PACKET PERMISSION  GRANTED to MOBILE " << *(_mobile_id) << " at " << sc_time_stamp().to_seconds() << endl;
				cout << "     MOBILE " << *(_mobile_id) << " STARTING TRANSMISSION at " << sc_time_stamp().to_seconds() << endl;
				transmitting = true;
				start_transmission_out.write(1);
				wait(8, SC_MS);
				cout << "     MOBILE " << *(_mobile_id) << " DONE TRANSMISSION! at " << sc_time_stamp().to_seconds() << endl;
				packet_counter++;
				transmitting = false;
				packet_request_out.write(0);
				start_transmission_out.write(0);
				packet_full.write(0);
				tuple_counter = 0;
				for (int i = 0; i < PACKET_SIZE; i++){

					for (int j = 0; j < tuple_NUM_COLUMNS; j++){

						TUPLE_ARRAY[i][j] = 0;

					}
				}
				//break;
			}
			else
			{
				cout << "     PACKET PERMISSION NOT GRANTED to MOBILE " << *(_mobile_id) << "at " << sc_time_stamp().to_seconds() << endl;
				wait(8, SC_MS);
			}
		}
		else{
			wait(8, SC_MS);
		}
	}
}


// SC_METHOD triggered on done_in
//void MOBILE<mobile_T>::transmission_done(){
//
//	if (done_in.read() == 1)
//	{
//		cout << "MOBILE " << *(_mobile_id) << " IS DONE TRANSMITTING" << endl;
//		
//		packet_counter++;
//		transmitting = false;
//		packet_request_out.write(0);
//		start_transmission_out.write(0);
//		tuple_counter = 0;
//		for (int i = 0; i < PACKET_SIZE; i++){
//
//			for (int j = 0; j < tuple_NUM_COLUMNS; j++){
//
//				TUPLE_ARRAY[i][j] = 0;
//
//			}
//		}
//
//	}
//
//
//
//
//}



void MOBILE<mobile_T>::print_mobile(){
	//cout << "SIMULATION TIME:\t" << sc_time_stamp().to_seconds() << "s" << endl;
	cout << "=================================================" << endl;
	cout << "|\t\tMOBILE " << *(_mobile_id) << " at " << sc_time_stamp().to_seconds() << "\t\t| " << endl;
	cout << "=================================================" << endl;
	cout << "| TUPLE COUNTER: \t" << tuple_counter << "\t\t\t|" <<endl;
	cout << "| PACKET COUNTER:\t" << packet_counter << "\t\t\t|" <<endl;
	cout << "| PACKET FULL:   \t" << packet_full.read() << "\t\t\t|" << endl;
	cout << "| TRANSMITTING:   \t" << transmitting << "\t\t\t|" << endl;
	cout << "=================================================" << endl;
	cout << "| SERVER FREE:   \t" << free_in.read() << "\t\t\t|" << endl;
	cout << "| PACKET REQ:    \t" << packet_request_out.read() << "\t\t\t|" << endl;
	cout << "| PACKET PERM:   \t" << packet_permission_in.read() << "\t\t\t|" << endl;
	cout << "| START TRANSM:  \t" << start_transmission_out.read() << "\t\t\t|" << endl;
	//cout << "| DONE:          \t" << done_in.read() << "\t\t\t|" << endl;
	cout << "=================================================" << endl;


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
	cout << "=========================================" << endl;
	*/



}
