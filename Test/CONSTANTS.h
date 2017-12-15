#include "systemc.h"
#define _CRT_SECURE_NO_WARNINGS
#ifndef CONSTANTS
#define CONSTANTS
using namespace std;

const int NUM_ROI = 5;
const int NUM_IMAGES = 5;

const int tuple_NUM_COLUMNS = 3;
const int PACKET_SIZE = 20;

const int NUM_MOBILES = 3;
const int MAX_TUPPLE_SIZE = 20;
const int SERVER_ARRAY_NUM_COLUMNS = 5;
const double BANDWIDTH_512KBS = 524288;
const double BANDWIDTH_1MBS = 1048576;
const double BANDWIDTH_5MBS = 5242880;
const double BANDWIDTH_10MBS = 10485760;
const double SERVER_TO_MOBILE_PACKET_1MB = 1048576;
const double SERVER_TO_MOBILE_PACKET_10MB = 10485760;
const double IMAGE_SIZE					= 8388608;


// TUPLE SIZE
const double TUPLE_SIZE = 192;	// 3 * 64

// RECEIVE PACKET SIZE
const double TRANSMIT_PACKET_SIZE = 3840;	// 20 * 3 * 64
const double IMAGE1_TIME = 100.00;
const double IMAGE2_TIME = 120.00;
const double IMAGE3_TIME = 125.00;
const double IMAGE4_TIME = 130.00;
const double IMAGE5_TIME = 150.00;

#endif 