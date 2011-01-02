/*
 * RLE.cpp
 *
 *  Created on: 09-12-2010
 *      Author: szkudi
 */

#include "RLE.h"
#include "SecondStepAlg.h"
#include <iostream>

using namespace std;
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

void RLE::encodeBuf(coderData* data){

	uint8_t counter = 1;
	uint8_t c = data->in_buf[0];
	int out_pos = 0;
//	int sum = 0;

	data->out_buf = new uint8_t[2 * data->in_size + 2 * sizeof(uint32_t)];

	uint8_t* out = data->out_buf + 2 * sizeof(uint32_t);

	for(unsigned int i = 1; i < data->in_size; ++i){
		if(counter < 255 && data->in_buf[i] == c)
			counter++;
		else {
			out[out_pos++] = counter;
//			cout << i << " count " << (int)counter << " znak = " << (int)c << endl;
//			sum += counter;
			out[out_pos++] = c;
			counter = 1;
			c = data->in_buf[i];
		}
	}
	out[out_pos++] = counter;
	out[out_pos++] = c;

	data->out_size = out_pos + 2 * sizeof(uint32_t);
	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = out_pos;
	(reinterpret_cast<uint32_t*>(data->out_buf))[1] = data->in_size;

//	cout << "In = " << data->in_size << " out = " << out_pos << " all = " << sum << endl;
}

void RLE::decodeBuf(coderData* data){

	int out_pos = 0;
	uint8_t cnt = 0;

	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[1];
	data->var_1 = (reinterpret_cast<uint32_t*>(data->in_buf))[0];

	data->out_buf = new uint8_t[data->out_size];

	uint8_t* in = data->in_buf + 2 * sizeof(uint32_t);

	for(int i = 0; i < data->var_1; i+=2){
		cnt = in[i];
//		cout << i << " " << out_pos << " count " << (int)cnt << " znak = " << (int)in[i + 1] << endl;
		while(cnt--){
			data->out_buf[out_pos++] = in[i + 1];
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
