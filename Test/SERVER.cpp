#include "SERVER.h"
#include <stdlib.h>    
#include <time.h>

typedef int server_T;

void SERVER<server_T>::prc_server(){


	/*for (int i = 0; i < NUM_MOBILES; i++)
	{
		if (packet_request_in[i].read() == 1)
		{
			server_is_free.write(1);
			
		}
		else{
			server_is_free.write(0);
			break;
		}
	}*/


	
		

}

// SC_METHOD triggered for each mobile's packet_request_in[i]
void SERVER<server_T>::prc_receive_from_mobile(){
	cout << "SERVER prc_receive_from_mobile() triggered at " << sc_time_stamp().to_seconds() << endl;
	for (int i = 0; i < NUM_MOBILES; i++)
	{
		
		if (packet_request_in[i].read() == 1 && server_is_free.read() == 1 && free_out[i].read() ==1)
		{
			cout << " SERVER IS GRANTING PERMISSION TO MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;
		
			transmitting.write(1);
			is_transmitting = true;
			packet_permission_out[i].write(1);
			free_out[i].write(1);
			server_is_free.write(0);
			
			for (int j = 0; j < NUM_MOBILES; j++)
			{

				// TELL OTHER MOBILES THAT THE SERVER IS OCCUPIED
				if (i != j)
				{
					cout << " SERVER IS TELLING MOBILE " << j << " DAT IT IZ NOT FREE at " << sc_time_stamp().to_seconds() << endl;
					free_out[j].write(0);

				}

			}
			cout << " SERVER IS BREKAING OUT OF prc_receive_from_mobile() " << i << " at " << sc_time_stamp().to_seconds() << endl;
			break;
		}
		else{
			cout << "SERVER SAID NAW PERMISSION TO MOBILE " << i << " at " << sc_time_stamp().to_seconds() << endl;
			packet_permission_out[i].write(0);
			server_is_free.write(1);
			//free_out[i].write(1);
		}

	}
	

}

//// SC_THREAD triggered for each mobile's start_packet_transmission_in[i]
//void SERVER<server_T>::prc_start_transmission(){
//	
//	cout << "SERVER prc_start_transmission() triggered" << endl;
//	
//	while (transmitting.read() == false)
//	{
//		cout << " INSIDE SERVER " << "prc_start_transmission() WHILE LOOP" << endl;
//		for (int i = 0; i < NUM_MOBILES; i++)
//		{
//					if (start_transmission_in[i].read() == 1)
//					{
//						
//
//						//transmitting = true;
//						transmitting.write(1);
//						cout << "   SERVER STARTING TRANSMISSION for MOBILE " << i << endl;
//						wait(8, SC_MS);
//						cout << "   SERVER DONE TRANSMISSION for MOBILE " << i << endl;
//
//						// PHASE 3 PLACEHOLDER
//						// PHASE 3 PLACEHOLDER
//						// PHASE 3 PLACEHOLDER 
//						// PHASE 3 PLACEHOLDER
//
//
//						cout << "   SERVER TELLS MOBILE " << i << " THAT IS DONE TRANSMITTING" << endl;
//						done_out[i].write(1);
//						packet_permission_out[i].write(0);
//
//						for (int j = 0; j < NUM_MOBILES; j++)
//						{
//							free_out[j].write(1);
//						}
//						server_is_free.write(1);
//						//cout << " BREAKING SERVER " << "prc_start_transmission() WHILE LOOP for MOBILE " << i << endl;
//						//break;
//						transmitting.write(0);
//
//					}
//					else{
//						wait(8, SC_MS);
//					}
//				}
//				
//					//else{
//					//	cout << "   SERVER NOT WAITING DURING TRANSMISSION" << endl;
//					//	wait(8, SC_MS);
//					//}
//			/*else
//			{
//			
//			done_out[i].write(0);
//
//		
//			}*/
//			
//
//	}
//
//		cout << "LEAVING SERVER " << "prc_start_transmission() "<< endl;
//}	


// SC_THREAD(prc_transmit);
//sensitive << transmitting.posedge_event();
void SERVER<server_T>::prc_transmit(){
	cout << "SERVER transmitting() triggered at " << sc_time_stamp().to_seconds() << endl;

	while (true){
		cout << " INSIDE SERVER transmitting() triggered WHILE LOOP at " << sc_time_stamp().to_seconds() << endl;
		if (is_transmitting == true){
			
			cout << "   SERVER STARTING TRANSMISSION for MOBILE  at " << sc_time_stamp().to_seconds() << endl;
			wait(24, SC_MS);
			cout << "   SERVER DONE TRANSMISSION for MOBILE  at " << sc_time_stamp().to_seconds() << endl;
			is_transmitting = false;
			for (int i = 0; i < NUM_MOBILES; i++)
			{
				
				packet_permission_out[i].write(0);
				free_out[i].write(1);
			}

			server_is_free.write(1);
			transmitting.write(0);
			
		}
		else{
			wait(8, SC_MS);


		}
		
		

	}
	cout << "LEAVING SERVER transmitting() at " << sc_time_stamp().to_seconds() << endl;

}

void SERVER<server_T>::print_server(){

	cout << "=================================================" << endl;
	cout << "|\t\tSERVER at " << sc_time_stamp().to_seconds() << "\t\t\t| " << endl;
	cout << "=================================================" << endl;
	cout << "| SERVER FREE STATUS: " << server_is_free << endl;
	cout << "| TRANSMITTING:       " << transmitting.read() << endl;
	cout << "=================================================" << endl;
	// THEN READ DATA STRUCTURE FROM LOCAL ARRAYS
	cout << "|MOBILE\t|FREE\t|REQ\t|PERM\t|START\t|DONE\t|" << endl;
	for (int i = 0; i < NUM_MOBILES; i++){
		server_array[i][0] = i;
		server_array[i][1] = free_out[i].read();
		server_array[i][2] = packet_request_in[i].read();
		server_array[i][3] = packet_permission_out[i].read();
		server_array[i][4] = start_transmission_in[i].read();
		server_array[i][5] = done_out[i].read();
		cout << "|";
		for (int j = 0; j < SERVER_ARRAY_NUM_COLUMNS; j++){
			cout << server_array[i][j] << "\t|";
		}
		cout << endl;
	}
	cout << "=================================================" << endl;


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