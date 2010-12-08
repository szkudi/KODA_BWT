/*
 * IF.cpp
 *
 *  Created on: 03-12-2010
 *      Author: szkudi
 */

#include "IF.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

namespace SecondStepAlgs {

IF::IF() {
}

IF::~IF() {
}

void IF::encodeBuf(const uint8_t *in_buf, unsigned int *out_buf, int buf_size){

	init();

	for(int i = 0; i < buf_size; ++i){
		if(!num_elem[in_buf[i]]++){
			ret_value[in_buf[i]].push_back(i);
			modifyTempElements(in_buf[i]);
		}else {
			ret_value[in_buf[i]].push_back(actual_dist[in_buf[i]]);
			modifyTempElements(in_buf[i]);
		}
	}

	int k = 0;
	for(int i = 0; i < alphabet_size; ++i){
		for(unsigned int j = 0; j < ret_value[i].size(); ++j){
			out_buf[k++] = ret_value[i][j];
		}
	}
}

void IF::decodeBuf(const unsigned int *in_buf, uint8_t *out_buf, int buf_size){

	memset(out_buf, 255, sizeof(uint8_t) * buf_size);

	int in = 0;//input buffer index
	int out = 0; //output buffer index
	int tmp;

	cout << "Begin" << endl;
	for(int i = 0; i < alphabet_size; ++i){ //for each alphabet symbol
		if(num_elem[i] > 0){
			out = in_buf[in];
			out_buf[out++] = i;
			for(int j = 1; j < num_elem[i]; ++j){ //for each occurrence of that symbol
				tmp = in_buf[in + j];
				if(tmp == 0){
					while(out_buf[out] <= i)
						out++;
				}else while(tmp > 0){
					if(out_buf[out++] > i){
						tmp--;
					}
				}
				out_buf[out] = i;
			}
			in += num_elem[i];
		}
	}

}

void IF::setNum_elem(const int *start){
	memcpy(num_elem, start, sizeof(int) * alphabet_size);
}

void IF::init(){
	memset(num_elem, 0, sizeof(int) * alphabet_size);
	memset(actual_dist, 0, sizeof(int) * alphabet_size);

	for(int i = 0; i < alphabet_size; ++i)
		ret_value.push_back(vector<int>());
}

int *IF::getNum_elem() const{
	int *tmp =  new int[alphabet_size];
	memcpy(tmp, num_elem, sizeof(int) * alphabet_size);

	return tmp;
}

void IF::modifyTempElements(uint8_t elem){
	actual_dist[elem] = 0;
	for(int i = 0; i < elem; ++i){
		actual_dist[i]++;
	}
}

}
