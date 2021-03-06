#include "MOBILE.h"
#include <iostream>
#include <iomanip>
typedef int mobile_T;

void MOBILE<mobile_T>::prc_mobile(){

	if (!transmitting && packet_full.read() == 0){

	
		currentImageIndex = receive_new_image_in.read();

		if (currentImageIndex > 0 && image_counter != currentImageIndex)
		{
			cout << "MOBILE " << *(_mobile_id) << " HAS READ X = " << randX.read() << " Y = " << randY.read() << " at " << sc_time_stamp().to_seconds() << " from IMAGE " << currentImageIndex;
			int within_this_ROI = 0;
			for (int i = 0; i < NUM_ROI; i++)
			{
				
				// CHECK IF X AND Y IS WITHIN THIS REGION OF INTEREST
				if (((LEFT_BOTTOM_X[i][currentImageIndex] <= randX.read()) && (randX.read() <= RIGHT_TOP_X[i][currentImageIndex])) &&
					((LEFT_BOTTOM_Y[i][currentImageIndex] <= randY.read()) && (randY.read() <= RIGHT_TOP_Y[i][currentImageIndex]))
					)
				{
					within_this_ROI = i + 1;
					cout << " (WITHIN ROI " << i + 1  << ")" << endl;
					//cout << "WRITING A POSEDGE TO ROI" << i + 1 << endl;
					ROI_INDEX_SIG[i].write(true);
				}
				else {
					
					//cout << "WRITING A NEGEDGE TO ROI" << i + 1 << endl;
					ROI_INDEX_SIG[i].write(false);
				}
			}
			if (within_this_ROI > 0 )
				cout << " (WITHIN ROI " << within_this_ROI << ")" << endl;

		}

	}
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
				cout << "MOBILE " << *(_mobile_id) << " PACKET IS FULL at " << sc_time_stamp().to_seconds() << endl;
				packet_full.write(1);

			}
		}
	}
}


// SC_THREAD triggered on packet_full
void MOBILE<mobile_T>::prc_request_to_server(){
	//cout << "MOBILE " << *(_mobile_id) << " prc_request_to_server() triggered at " << sc_time_stamp().to_seconds() << endl;

	while (true)
	{
		//cout << " INSIDE MOBILE " << *(_mobile_id) << " prc_request_to_server() WHILE LOOP at " << sc_time_stamp().to_seconds() << endl;
		// SERVER IS FREE AND PACKET IS FULL
		if (free_in.read() == 1 && packet_full.read() == 1 && tuple_counter == PACKET_SIZE)
		{
			cout << "  MOBILE " << *(_mobile_id) << " SEES THAT SERVER IS FREE at " << sc_time_stamp().to_seconds() << endl;

			if (transmitting == false)
			{

				packet_request_out.write(1);
				cout << "   MOBILE " << *(_mobile_id) << " IS REQUESTING SERVER at " << sc_time_stamp().to_seconds() << endl;
				wait(4, SC_MS);

			}

			if (packet_permission_in.read() == 1 && transmitting == false)
			{
				cout << "     PACKET PERMISSION  GRANTED to MOBILE " << *(_mobile_id) << " at " << sc_time_stamp().to_seconds() << endl;
				cout << "     MOBILE " << *(_mobile_id) << " STARTING TRANSMISSION at " << sc_time_stamp().to_seconds() << endl;
				mobile_file1 << std::fixed << std::setprecision(3) << sc_time_stamp().to_seconds() << "s ";
				//mobile_file1 << "\tImage " << currentImageIndex << " packet transmitted" << endl;
				mobile_file1 << "\t10" << endl;
				transmitting = true;
				start_transmission_out.write(1);
				//wait(8, SC_MS);
				wait(transmission_time, SC_SEC);
				cout << "     MOBILE " << *(_mobile_id) << " DONE TRANSMISSION! at " << sc_time_stamp().to_seconds() << endl;
				mobile_file1 << std::fixed << std::setprecision(3) << sc_time_stamp().to_seconds() << "s ";
				//mobile_file1 << "\tImage " << currentImageIndex << " packet received" << endl;
				mobile_file1 << "\t5" << endl;
				if (image_transmitted_done_in.read() == 1)
				{
					image_counter++;
					packet_counter = -1;
					receive_packet_counter = -1;
				}

				receive_packet_counter++;
				packet_counter++;

				transmitting = false;
				packet_request_out.write(0);
				start_transmission_out.write(0);
				packet_full.write(0);
				tuple_counter = 0;
				for (int i = 0; i < PACKET_SIZE; i++)
				{
					for (int j = 0; j < tuple_NUM_COLUMNS; j++)
					{

						TUPLE_ARRAY[i][j] = 0;
					}
				}

			}
			else
			{
				packet_request_out.write(0);
				cout << "     PACKET PERMISSION NOT GRANTED to MOBILE " << *(_mobile_id) << " at " << sc_time_stamp().to_seconds() << endl;
				wait(8, SC_MS);
			}
		}
		else
		{
			wait(8, SC_MS);
		}
	}
}


void MOBILE<mobile_T>::print_mobile(){

	if (currentImageIndex > 0)
	{
		cout << "=================================================" << endl;
		cout << "|\t\tMOBILE " << *(_mobile_id) << " at " << sc_time_stamp().to_seconds() << "\t\t| " << endl;
		cout << "=================================================" << endl;
		cout << "| IMAGE INDEX:   \t" << currentImageIndex << "\t\t\t|" << endl;
		cout << "| IMAGE COUNTER: \t" << image_counter << "\t\t\t|" << endl;
		cout << "| TUPLE COUNTER: \t" << tuple_counter << "\t\t\t|" << endl;
		cout << "| PACKET COUNTER:\t" << packet_counter << "\t\t\t|" << endl;
		cout << "| PACKET FULL:   \t" << packet_full.read() << "\t\t\t|" << endl;
		cout << "| TRANSMITTING:  \t" << transmitting << "\t\t\t|" << endl;
		cout << "=================================================" << endl;
		cout << "| SERVER FREE:   \t" << free_in.read() << "\t\t\t|" << endl;
		cout << "| PACKET REQ:    \t" << packet_request_out.read() << "\t\t\t|" << endl;
		cout << "| PACKET PERM:   \t" << packet_permission_in.read() << "\t\t\t|" << endl;
		cout << "| START TRANSM:  \t" << start_transmission_out.read() << "\t\t\t|" << endl;
		cout << "=================================================" << endl;


		mobile_file2 << std::fixed << std::setprecision(3) << sc_time_stamp().to_seconds();
		mobile_file2 << std::fixed << std::setprecision(0) << "\t" << (tuple_counter * TUPLE_SIZE) +
			(packet_counter * TRANSMIT_PACKET_SIZE) +
			(receive_packet_counter * SERVER_TO_MOBILE_PACKET_10MB) << endl;
																	//(image_counter * IMAGE_SIZE) << endl;
		//"\ttc: " << tuple_counter << "\tpc: " << packet_counter << "\trpc: " << receive_packet_counter << "\tic: " << image_counter << "\tcurrentImage: " << currentImageIndex << endl;
	}

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