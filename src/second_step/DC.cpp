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

void DC::encodeBuf(const uint8_t* in_buf, unsigned int* out_buf, int buf_size){

//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

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
}

void DC::decodeBuf(const unsigned int* in_buf, uint8_t* out_buf, int buf_size){

	int j;
	int tmp;
//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

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
