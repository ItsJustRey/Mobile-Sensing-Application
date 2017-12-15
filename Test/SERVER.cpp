#include "SERVER.h"
#include <stdlib.h>    
#include <time.h>

typedef int server_T;

void SERVER<server_T>::send_new_image(){

	double currentSimulationTime = sc_time_stamp().to_seconds();
	if (currentSimulationTime >= IMAGE1_TIME && currentSimulationTime < IMAGE2_TIME)
	{
		currentImageIndex = 1;
		//cout << " SERVER IS SENDING IMAGE " << currentImageIndex << " at " << sc_time_stamp().to_seconds() << endl;
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			send_new_image_out[i].write(1);
		}

	}
	else if (currentSimulationTime >= IMAGE2_TIME && currentSimulationTime < IMAGE3_TIME)
	{
		currentImageIndex = 2;
		//cout << " SERVER IS SENDING IMAGE " << currentImageIndex << " at " << sc_time_stamp().to_seconds() << endl;
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			send_new_image_out[i].write(2);
		}

	}
	else if (currentSimulationTime >= IMAGE3_TIME && currentSimulationTime < IMAGE4_TIME)
	{
		currentImageIndex = 3;
		//cout << " SERVER IS SENDING IMAGE " << currentImageIndex << " at " << sc_time_stamp().to_seconds() << endl;
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			send_new_image_out[i].write(3);
		}

	}
	else if (currentSimulationTime >= IMAGE4_TIME && currentSimulationTime < IMAGE5_TIME)
	{
		currentImageIndex = 4;
		//cout << " SERVER IS SENDING IMAGE " << currentImageIndex << " at " << sc_time_stamp().to_seconds() << endl;
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			send_new_image_out[i].write(4);
		}

	}
	else if (currentSimulationTime >= IMAGE5_TIME)
	{
		currentImageIndex = 5;
		//cout << " SERVER IS SENDING IMAGE " << currentImageIndex << " at " << sc_time_stamp().to_seconds() << endl;
		for (int i = 0; i < NUM_MOBILES; i++)
		{
			send_new_image_out[i].write(5);
		}

	}
}


// SC_METHOD triggered for each mobile's packet_request_in[i]
void SERVER<server_T>::prc_receive_from_mobile(){
	cout << "SERVER prc_receive_from_mobile() triggered at " << sc_time_stamp().to_seconds() << endl;
	for (int i = 0; i < NUM_MOBILES; i++)
	{
		if (packet_request_in[i].read() == 1)
		{
			if (server_is_free == true && free_out[i].read() == 1 && is_transmitting == false)
			{
				cout << " SERVER IS GIVING PERMISSION TO MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;
				transmitting[i].write(1);
				is_transmitting = true;
				packet_permission_out[i].write(1);
				free_out[i].write(1);
				server_is_free = false;

				for (int j = 0; j < NUM_MOBILES; j++)
				{
					// TELL OTHER MOBILES THAT THE SERVER IS OCCUPIED
					if (i != j)
					{
						cout << " SERVER IS TELLING MOBILE " << j << " THAT IT IS NOT FREE at " << sc_time_stamp().to_seconds() << endl;
						free_out[j].write(0);
					}
				}
			}

			else
			{
				cout << "SERVER DID NOT GRANT PERMISSION TO MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;
				packet_permission_out[i].write(0);
				//server_is_free.write(1);
				server_is_free = true;
			}
		}
	}
}

// SC_THREAD(prc_transmit);
//sensitive << transmitting[i].posedge_event();
void SERVER<server_T>::prc_transmit(){
	//cout << "SERVER transmitting() triggered at " << sc_time_stamp().to_seconds() << endl;

	while (true)
	{
		
		//cout << " INSIDE SERVER transmitting() triggered WHILE LOOP at " << sc_time_stamp().to_seconds() << endl;
		if (is_transmitting == true)
		{
			for (int i = 0; i < NUM_MOBILES; i++)
			{
				if (transmitting[i].read() == 1)
				{
					cout << "   SERVER STARTING TRANSMISSION for MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;

					//wait(16, SC_MS);
					wait(transmission_time, SC_SEC);

					cout << "   SERVER DONE TRANSMISSION for MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;

					image_bits_transmitted[i] += packet_size;
					// IF IMAGE DONE TRANSMITTING, TELL THIS MOBILE, AND RESET 
					if (image_bits_transmitted[i] >= IMAGE_SIZE)
					{
						image_transmitted_done_out[i].write(1);
						image_bits_transmitted[i] = 0;
					}
					else{
						image_transmitted_done_out[i].write(0);

					}

					// TELL ALL MOBILES
					for (int j = 0; j < NUM_MOBILES; j++)
					{
						packet_permission_out[j].write(0);
						free_out[j].write(1);
					}

					is_transmitting = false;
					server_is_free = true;
					transmitting[i].write(0);
				}
			}
		}
		else
		{
			wait(8, SC_MS);
			/*for (int i = 0; i < NUM_MOBILES; i++)
			{
				image_transmitted_done_out[i].write(0);

			}*/
			
		}
	}
	//cout << "LEAVING SERVER transmitting() at " << sc_time_stamp().to_seconds() << endl;

}


//void SERVER<server_T>::image_transmitted_done(){
//
//	for (int i = 0; i < NUM_MOBILES; i++)
//	{
//		image_transmitted_done_out[i].write(1);
//
//
//}




void SERVER<server_T>::print_server(){
	if (currentImageIndex > 0)
	{
		cout << "=========================================" << endl;
		cout << "|\t\tSERVER at " << sc_time_stamp().to_seconds() << "\t\t| " << endl;
		cout << "=========================================" << endl;
		cout << "| SERVER FREE STATUS: " << server_is_free << "\t\t\t|" << endl;
		cout << "| TRANSMITTING:       " << (transmitting[0].read() || transmitting[1].read() || transmitting[0].read() ) << "\t\t\t|" << endl;
		cout << "=========================================" << endl;
		// THEN READ DATA STRUCTURE FROM LOCAL ARRAYS
		cout << "|MOBILE\t|FREE\t|REQ\t|PERM\t|START\t|" << endl;
		for (int i = 0; i < NUM_MOBILES; i++){
			server_array[i][0] = i;
			server_array[i][1] = free_out[i].read();
			server_array[i][2] = packet_request_in[i].read();
			server_array[i][3] = packet_permission_out[i].read();
			server_array[i][4] = start_transmission_in[i].read();
			cout << "|";
			for (int j = 0; j < SERVER_ARRAY_NUM_COLUMNS; j++){
				cout << server_array[i][j] << "\t|";
			}
			cout << endl;
		}
		cout << "=========================================" << endl;

	}
	/*cout << endl << "======Tuple ARRAY in SERVER=======" << endl;
	cout << "===========================================================================" << endl;
	cout << "|\t MOBILE 0 \t\t MOBILE 1 \t\t MOBILE 2 \t|" << endl;
	cout << "===========================================================================" << endl;
	cout << "|ROI\t|START\t|END\t|ROI\t|START\t|END\tROI\t|START\t|END\t|" << endl;
	cout << "===========================================================================" << endl;
	for (int i = 0; i < MAX_TUPPLE_SIZE; i++){

	cout << "|";
	for (int j = 0; j < NUM_MOBILES * 3; j++){
	cout << TUPPLE_ARRAY[i][j] << "\t|";
	}
	cout << endl;
	}
	cout << "=========================================" << endl;*/

}