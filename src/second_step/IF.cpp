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

void IF::encodeBuf(const uint8_t *in_buf, int32_t *out_buf, int buf_size){

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

	int out_pos = 1;
	int count = 0;

	for(int i = 0; i < alphabet_size; ++i){
		if(num_elem[i] > 0){
			count++;
			out_buf[out_pos++] = i;
			out_buf[out_pos++] = num_elem[i];
		}
	}

	out_buf[0] = count;

	for(int i = 0; i < alphabet_size; ++i){
		for(unsigned int j = 0; j < ret_value[i].size(); ++j){
			out_buf[out_pos++] = ret_value[i][j];
		}
	}
}

void IF::decodeBuf(const int32_t *in_buf, uint8_t *out_buf, int buf_size){


	int in = 1;//input buffer index
	int out = 0; //output buffer index
	int tmp;

	init();

	int count = in_buf[0];
//	cout << count << endl;
	for(int i = 0; i < count; ++i){
		num_elem[in_buf[in]] = in_buf[in + 1];
		in += 2;
	}

	memset(out_buf, 255, sizeof(uint8_t) * buf_size);


//	cout << "Begin " <<  in << endl;
	for(int i = 0; i < alphabet_size; ++i){ //for each alphabet symbol
		if(num_elem[i] > 0){
//			cout << i << " " << in << " num = " << num_elem[i] <<  endl;
			out = in_buf[in];
//			cout << out << endl;
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
//				cout << out << endl;
			}
			in += num_elem[i];
		}
	}

}

void IF::setNum_elem(const int32_t *start){
	memcpy(num_elem, start, sizeof(int) * alphabet_size);
}

void IF::init(){
	memset(num_elem, 0, sizeof(int) * alphabet_size);
	memset(actual_dist, 0, sizeof(int) * alphabet_size);

	for(int i = 0; i < alphabet_size; ++i)
		ret_value.push_back(vector<int32_t>());
}

int32_t *IF::getNum_elem() const{
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
