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

	for (int i = 0; i < NUM_MOBILES; i++)
	{
		
		if (packet_request_in[i].read() == 1 && server_is_free.read() == 1 && free_out[i].read() ==1)
		{
			cout << "SERVER IS GRANTING PERMISSION TO MOBILE " << i << endl;
			packet_permission_out[i].write(1);
			free_out[i].write(1);
			server_is_free.write(0);
			for (int j = 0; j < NUM_MOBILES; j++)
			{

				// TELL OTHER MOBILES THAT THE SERVER IS OCCUPIED
				if (i != j)
				{
					cout << "SERVER IS TELLING MOBILE " << j << " DAT IT IZ NOT FREE" << endl;
					free_out[j].write(0);

				}

			}
			break;
		}
		else{
			cout << "SERVER SAID NAW PERMISSION TO MOBILE " << i << endl;
			packet_permission_out[i].write(0);
			server_is_free.write(1);
			//free_out[i].write(1);
		}

	}
	

}

// SC_THREAD triggered for each mobile's start_packet_transmission_in[i]
void SERVER<server_T>::prc_start_transmission(){

	for (int i = 0; i < NUM_MOBILES; i++)
	{
		if (start_transmission_in[i].read() == 1)
		{
			
			wait(8, SC_MS);

			// PHASE 3 PLACEHOLDER
			// PHASE 3 PLACEHOLDER
			// PHASE 3 PLACEHOLDER 
			// PHASE 3 PLACEHOLDER


			cout << "MOBILE " << i << " DONE TRANSMISSION" << endl;
			done_out[i].write(1);


			for (int j = 0; j < NUM_MOBILES; j++)
			{
				free_out[j].write(1);
			}
			server_is_free.write(1);
			
		}
	}

	
}



void SERVER<server_T>::print_server(){

	cout << endl << "~~~~~~~~~~~~~~~~~Server~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "SERVER IS FREE: " << server_is_free << endl;
	cout << "=================================================" << endl;
	// THEN READ DATA STRUCTURE FROM LOCAL ARRAYS
	cout << "|MOBILE\t|PR\t|FREE\t|DONE\t|" << endl;
	for (int i = 0; i < NUM_MOBILES; i++){
		server_array[i][0] = i;
		server_array[i][1] = packet_request_in[i];
		server_array[i][2] = free_out[i].read();
		server_array[i][3] = done_out[i].read();
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