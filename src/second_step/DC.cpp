/*
 * DC.cpp
 *
 *  Created on: 01-12-2010
 *      Author: szkudi
 */

#include "DC.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <cstdio>

namespace SecondStepAlgs {

using namespace std;

DC::DC(){//:SecondStepAlg() {
}

DC::~DC() {
}

int DC::encodeBuf(const uint8_t* in_buf, uint32_t* out_buf, int buf_size){

//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);
	int count = 0;
	int out_pos = buf_size + 1;

	init();

	for(int i = 0; i < buf_size; ++i){
//		cout << i << " " << in_buf[i];
//		printf(" %d\n", num_elem[in_buf[i]]);
		if(start_pos[in_buf[i]] == -1){
			start_pos[in_buf[i]] = i;
			last_pos[in_buf[i]] = i;
			out_buf[i] = 0;
		}
		else {
//			cout << actual_dist[in_buf[i]] << " " << in_buf[i] << endl;
			out_buf[last_pos[in_buf[i]]] = i - last_pos[in_buf[i]];
			last_pos[in_buf[i]] = i;
			out_buf[i] = 0;
		}
	}

	for(int i = 0; i < alphabet_size; ++i){
		if(start_pos[i] > -1){
			count++;
			out_buf[out_pos++] = i;
			out_buf[out_pos++] = start_pos[i];
		}
	}
	out_buf[buf_size] = count;

	return out_pos;
}

void DC::decodeBuf(const uint32_t* in_buf, uint8_t* out_buf, int buf_size){

	int j;
	int tmp;
//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

	init();

	int count = in_buf[buf_size];
	int in_pos = buf_size + 1;

	for(int i = 0; i < count; ++i){
		start_pos[in_buf[in_pos]] = in_buf[in_pos + 1];
		in_pos += 2;
	}

	for(int i = 0; i < alphabet_size; ++i){
		if((j = start_pos[i]) >= 0){
//			cout << i <<  " " << j << endl;
			while(in_buf[j] != 0){
				tmp = in_buf[j];
				out_buf[j] = i;
				j += tmp;
			}
			out_buf[j] = i;
		}
	}

}

void DC::encodeBuf(coderData* data){

	int count = 0;
	int out_pos = data->in_size + 1;

	init();

	data->out_buf = new uint8_t[(data->in_size + alphabet_size + 1) * sizeof(uint32_t)];

	uint32_t* out = reinterpret_cast<uint32_t*>(data->out_buf) + 1;

	for(unsigned int i = 0; i < data->in_size; ++i){
		if(start_pos[data->in_buf[i]] == -1){
			start_pos[data->in_buf[i]] = i;
			last_pos[data->in_buf[i]] = i;
			out[i] = 0;
		}
		else {
//			cout << actual_dist[in_buf[i]] << " " << in_buf[i] << endl;
			out[last_pos[data->in_buf[i]]] = i - last_pos[data->in_buf[i]];
			last_pos[data->in_buf[i]] = i;
			out[i] = 0;
		}
	}

	for(int i = 0; i < alphabet_size; ++i){
		if(start_pos[i] > -1){
			count++;
			out[out_pos++] = i;
			out[out_pos++] = start_pos[i];
		}
	}
	out[data->in_size] = count;

	data->out_size = (out_pos + 1) * sizeof(uint32_t);

	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size;
}

void DC::decodeBuf(coderData* data){

	int j;
	int tmp;
//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

	init();

	uint32_t* in = reinterpret_cast<uint32_t*>(data->in_buf) + 1;
	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];

	data->out_buf = new uint8_t[data->out_size];

	int count = in[data->out_size];
	int in_pos = data->out_size + 1;

	for(int i = 0; i < count; ++i){
		start_pos[in[in_pos]] = in[in_pos + 1];
		in_pos += 2;
	}

	for(int i = 0; i < alphabet_size; ++i){
		if((j = start_pos[i]) >= 0){
//			cout << i <<  " " << j << endl;
			while(in[j] != 0){
				tmp = in[j];
				data->out_buf[j] = i;
				j += tmp;
			}
			data->out_buf[j] = i;
		}
	}

}

void DC::init(){
	memset(start_pos, -1, sizeof(int) * alphabet_size);
	memset(last_pos, -1, sizeof(int) * alphabet_size);
}

int* DC::getStart_dst() const {
	int *tmp = (int *)malloc(sizeof(int) * alphabet_size);
	memcpy(tmp, start_pos, sizeof(int) * alphabet_size);

	return tmp;
}

void DC::setStart_dst(const int* start){
	memcpy(start_pos, start, sizeof(int) * alphabet_size);
}

}
