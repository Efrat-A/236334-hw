#ifndef CRC_HPP_
#define CRC_HPP_
/*
 * param msg - message to encode
 * param len - length of msg
 * param p - polynom for the CRC
 * param g - degree of the polynom
 * out_param e_msg - encoded message (output parameter)
 * */
void encode(int* msg, int len, int* p, int g, int* e_msg);

/*
 * param msg - message to validate
 * param len - length of msg
 * param p - polynom for the CRC
 * param g - degree of the polynom
 * return true if msg is valid, false otherwise.
 * */
bool validate(int* msg, int len, int* p, int g);

#endif /* CRC_HPP_ */
