#include "crc.hpp"
#include <cstdlib>
#include <iostream>
#include <assert.h>
//#include <limits>
#include <iomanip>  // precision in printing output
#define NUM_SIMULATIONS 2000

void initiate(int * arr, int len) {
	for (int j = 0; j < len; j++)
		arr[j] = 0; // zeros
}
//just a random message, you can choose one of your one...
int msg[256] = { 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 0,
		0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1,
		0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1,
		1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 1,
		0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0,
		1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1,
		1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0,
		0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1,
		0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1,
		1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1 };
int p1[2] = { 1, 1 }; //1+x
int p2[65] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 }; // x^64
int p3[16] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 }; //1+x^14+x^15

//addition
int e_msg[400];
/*
 error mode = 0 drops no errors,
 1 one error,
 2 two errors
 */
//int hash[400];
//int history[400];
double sim_crc(int* msg, int len, int* p, int g, int error_mode) {
	initiate(e_msg, 400);
	int discovered(0);
	int rand_index1(-1),rand_index2(-1);

	for (int o = len + g - 1; o >= 0; --o)
		assert(e_msg[o]==0);

	encode(msg, len, p, g, e_msg);

	for (int o = len + g - 1; o >= g; --o)
		assert(msg[o-g]==e_msg[o]);

	for (int n = 0; n < NUM_SIMULATIONS; ++n) {

		// drop errors by selected mode
		if (error_mode >= 1) {
			rand_index1 = std::rand() % (len + g);
			e_msg[rand_index1] = e_msg[rand_index1] ^ 1;
		}
		if (error_mode >= 2) {
			rand_index2 = std::rand() % (len + g);
			while (rand_index1 == rand_index2) // if the errors are on the same indexes
				rand_index2 = std::rand() % (len + g);
			e_msg[rand_index2] = e_msg[rand_index2] ^ 1;
		}

//		history[rand_index1]++;
//		if (rand_index2 > 1)
//			history[rand_index2]++;

		if (validate(e_msg, len + g, p, g) == false) {

//			hash[rand_index1]++;
//			if (rand_index2 > 1)
//				hash[rand_index2]++;
			discovered++;
		}
		// return to original Tx, reverse action for next simulation
		if (error_mode >= 1) {
			e_msg[rand_index1] = e_msg[rand_index1] ^ 1;
		}
		if (error_mode >= 2) {
			e_msg[rand_index2] = e_msg[rand_index2] ^ 1;
		}
	}

//	std::cout << "\n not discovered: ";
//
//	for (int i = 0; i < len + g; ++i) {
//		if (history[i] > 0 && hash[i] == 0)
//			std::cout << "(" << i; //<< ":" << hash[i] <<")";
//		hash[i] = 0;
//		history[i] = 0;
//	}

	return discovered / (double) NUM_SIMULATIONS;
}

int main() {
	std::cout << std::setprecision(3) << std::fixed;
	std::cout << "Begin \n";
	std::cout << "\n[1]" << sim_crc(msg, 256, p1, 1, 1); // probability for x+1 with 1 error
	std::cout << "\n[0]" << sim_crc(msg, 256, p1, 1, 2); // probability for x+1 with 2 errors
	std::cout << "\n[0.2]" << sim_crc(msg, 256, p2, 64, 1); // probability for x^64 with 1 error
	std::cout << "\n[0.36]" << sim_crc(msg, 256, p2, 64, 2); // probability for x^64 with 2 errors
	std::cout << "\n[1]" << sim_crc(msg, 256, p3, 15, 1); // probability for x^15+x^14+1 with 1 error
	std::cout << "\n[1]" << sim_crc(msg, 256, p3, 15, 2); // probability for x^15+x^14+1 with 2 errors
	std::cout << "\nDone";

	return 0;
}
