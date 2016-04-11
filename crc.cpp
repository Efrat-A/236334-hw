#include "crc.hpp"
#include <assert.h>
#include <cstdlib>
#include <iostream>
/*
 * param msg - message to encode
 * param p - polynom for the CRC
 * param g - degree of the polynom
 * output - CRC of the messege
 * */
void encode(int* msg, int len, int* p, int g, int* e_msg) {
// some input validations
	if (!msg || len < 1 || !p || g < 1 || !e_msg) {
		return;
	}
	int msg_msb_index = len - 1;
	/****************************
	 Get the MSB of the message
	 ****************************/
	for (int i = len - 1; i >= 0; --i) {
		if (msg[i] == 1) {
			msg_msb_index = i;
			break;
		}
	}
	/****************************
	 Copy the message to e_msg, shifted g
	 ****************************/
	for (int x = 0; x < g; ++x)
		e_msg[x] = 0;
	for (int x = 0; x < len; ++x)
		e_msg[x + g] = msg[x];

	/****************************
	 Compute the reminder
	 ****************************/
	if (msg_msb_index < g) { // polynom is grater than msg
		for (int i = 0; i < g; ++i) {
			if (i <= msg_msb_index)
				e_msg[i] = msg[i];
			else
				e_msg[i] = 0;
		}
		return;
	}
	int msb_index = msg_msb_index + g; //e_msg msb 'on bit' index
	while (msb_index >= g) {
		for (int j = g; j >= 0; --j, --msb_index) // xor with poly
			e_msg[msb_index] = e_msg[msb_index] ^ p[j];
		msb_index += g + 1;
		while (e_msg[msb_index] == 0 && msb_index >= g) {
			msb_index--;
		}
	}
	/****************************
	 Finish e_msg, Attach msg
	 ****************************/
	for (int x = 0; x < len; ++x)
		e_msg[x + g] = msg[x];
}
/*
 msg and polynom,
 output - true\false Is the messege validate according to the polynom
 */
bool validate(int* msg, int len, int* p, int g) {
// msg include the reminder from last polynom
	if (!msg || !p || len < 1 || g < 1) {
		assert(0);
		return false;
	}
	int* test = (int*) malloc(sizeof(int) * (len + g));
	assert(test);

	encode((msg + g), len - g, p, g, test);
	for (int i = 0; i < g; ++i) {
		if (test[i] != msg[i]) {
			free(test);
			return false;
		}
	}
	free(test);
	return true;
}

