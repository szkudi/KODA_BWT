/*
 * RLE.cpp
 *
 *  Created on: 09-12-2010
 *      Author: szkudi
 */

#include "RLE.h"

namespace SecondStepAlgs {

RLE::RLE() {
}

RLE::~RLE() {
}

int RLE::encodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	uint8_t counter = 1;
	uint8_t c = in_buf[0];
	int out_pos = 0;

	for(int i = 1; i < buf_size; ++i){
		if(counter < 255 && in_buf[i] == c)
			counter++;
		else {
			out_buf[out_pos++] = counter;
			out_buf[out_pos++] = c;
			counter = 1;
			c = in_buf[i];
		}
	}
	out_buf[out_pos++] = counter;
	out_buf[out_pos++] = c;

	return out_pos;

}

void RLE::decodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	int out_pos = 0;
	uint8_t cnt = 0;

	for(int i = 0; i < buf_size; i+=2){
		cnt = in_buf[i];
		while(cnt--){
			out_buf[out_pos++] = in_buf[i + 1];
		}
	}

}

int RLE::getResizeLength(const uint8_t* in_buf, int buf_size){

	int count = 0;

	for(int i = 0; i < buf_size; i+=2)
		count += in_buf[i];

	return count;
}

}
