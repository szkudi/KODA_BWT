/*
 * RLE_2.cpp
 *
 *  Created on: 08-12-2010
 *      Author: szkudi
 */

#include "RLE_2.h"
#include <iostream>

using namespace std;

namespace SecondStepAlgs {

RLE_2::RLE_2() {
	// TODO Auto-generated constructor stub

}

RLE_2::~RLE_2() {
	// TODO Auto-generated destructor stub
}

int RLE_2::encodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){
	int run_start_index = 0;
	uint8_t run_symbol = 0xFF; //should be -1, but uint8_t can't handle it :)
	output_index = 0;

	bool first = true;


	int run_length;

	for(int i = 0; i < buf_size; ++i){
		if(!first && in_buf[i] == run_symbol){
			run_length = i - run_start_index + 1;
			if(run_length <= 2)
				out_buf[output_index++] = run_symbol;
		}else{
			run_length = i - run_start_index;
			if(run_length >= 2){
				RLE_buffer.push_back(run_length);
			}

			out_buf[output_index++] = in_buf[i];
			run_symbol = in_buf[i];
			run_start_index = i;

			first = false;
		}
	}

	run_length = buf_size - run_start_index;
	if(run_length >= 2){
		RLE_buffer.push_back(run_length);
	}

	return output_index + 1;

}

void RLE_2::decodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	uint8_t run_symbol;
	int output_index = 0;
	int rle_index = 0;

	bool first = false;

	for(int i = 0; i < buf_size; ++i){
		if(!first){
			run_symbol = in_buf[i];
			out_buf[output_index++] = run_symbol;
			first = true;
		} else if(in_buf[i] == run_symbol){
			for(int j = 0; j < RLE_buffer[rle_index] - 1; j++)
				out_buf[output_index++] = run_symbol;
			rle_index++;
			first = false;
		} else{
			run_symbol = in_buf[i];
		}

	}

}

int* RLE_2::getRLE_buffer() const{
	int* ret = new int[RLE_buffer.size()];

	for(unsigned int i = 0; i < RLE_buffer.size(); ++i)
		ret[i] = RLE_buffer[i];

	return ret;
}

int RLE_2::getSizeRLE_buffer() const{
	return (int)RLE_buffer.size();
}

int RLE_2::getResizeValueRLE_buffer() const{
	int resize = 0;

	for(unsigned int i = 0; i < RLE_buffer.size(); ++i){
		resize += RLE_buffer[i] - 2;
	}

	return resize;
}

void RLE_2::setRLE_buffer(const int* buffer, int size){
	RLE_buffer.clear();

	for(int i = 0; i < size; ++i)
		RLE_buffer.push_back(buffer[i]);
//	RLE_buffer = vector<int> (buffer, buffer + sizeof(int) * size);
}

}
