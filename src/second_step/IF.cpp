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

int IF::encodeBuf(const uint8_t *in_buf, uint32_t *out_buf, int buf_size){

	init();

	for(int i = 0; i < buf_size; ++i){
//		if(!num_elem[in_buf[i]]++){
//			ret_value[in_buf[i]].push_back(i);
////			cout << i << " znak = " << in_buf[i] << endl;
//			modifyTempElements(in_buf[i]);
//		}else {
//			ret_value[in_buf[i]].push_back(actual_dist[in_buf[i]]);
//			modifyTempElements(in_buf[i]);
//		}
		if(num_elem[in_buf[i]] == 0)
			ret_value[in_buf[i]].push_back(i);
		else
			ret_value[in_buf[i]].push_back(actual_dist[in_buf[i]]);
		num_elem[in_buf[i]]++;
		modifyTempElements(in_buf[i]);

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

	cout << count << endl;

	for(int i = 0; i < alphabet_size; ++i){
		cout << "znak = " << (char)i << endl;
		for(unsigned int j = 0; j < ret_value[i].size(); ++j){
			out_buf[out_pos++] = ret_value[i][j];
			cout << ret_value[i][j] << " " ;
		}
		cout << endl;
	}

	return out_pos;
}

void IF::decodeBuf(const uint32_t *in_buf, uint8_t *out_buf, int buf_size){


	int in = 1;//input buffer index
	int out = 0; //output buffer index
	int tmp;

	init(false);

	int count = in_buf[0];
	cout << count << endl;
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
			cout << "znak = " << (char)i << endl << out << " ";
//			cout << out << endl;
			out_buf[out++] = i;
			for(int j = 1; j < num_elem[i]; ++j){ //for each occurrence of that symbol
				tmp = in_buf[in + j];
//				cout << tmp << " ";
				if(tmp == 0){
					while(out_buf[out] < i)
						out++;
				}else
					while(tmp > 0){
						if(out_buf[out++] > i){
							tmp--;
					}
				}
				out_buf[out++] = i;
			}
//			cout << endl;
			in += num_elem[i];
		}
	}

}

void IF::encodeBuf(coderData* data){

	init();

	data->out_buf = new uint8_t[(data->in_size + alphabet_size + 2) * sizeof(uint32_t)];

	uint32_t* out = reinterpret_cast<uint32_t*>(data->out_buf);
	out[0] = data->in_size;


	for(unsigned int i = 0; i < data->in_size; ++i){
		if(num_elem[data->in_buf[i]] == 0)
			ret_value[data->in_buf[i]].push_back(i);
		else
			ret_value[data->in_buf[i]].push_back(actual_dist[data->in_buf[i]]);
		modifyTempElements(data->in_buf[i]);
		num_elem[data->in_buf[i]]++;
	}

	int out_pos = 2;
	int count = 0;

	for(int i = 0; i < alphabet_size; ++i){
		if(num_elem[i] > 0){
			count++;
			out[out_pos++] = i;
			out[out_pos++] = num_elem[i];
		}
	}

	out[1] = count;

	for(int i = 0; i < alphabet_size; ++i){

		for(unsigned int j = 0; j < ret_value[i].size(); ++j){
			out[out_pos++] = ret_value[i][j];
		}
	}

	data->out_size = out_pos * sizeof(uint32_t);
}

void IF::decodeBuf(coderData* data){


	int in_pos = 0;//input buffer index
	int out_pos = 0; //output buffer index
	int tmp;

	init(false);

	uint32_t* in = reinterpret_cast<uint32_t*>(data->in_buf);

	data->out_size = in[in_pos++];
	data->out_buf = new uint8_t[data->out_size];

	int count = in[in_pos++];
	for(int i = 0; i < count; ++i){
		num_elem[in[in_pos]] = in[in_pos + 1];
		in_pos += 2;
	}

	memset(data->out_buf, 255, sizeof(uint8_t) * data->out_size);


	for(int i = 0; i < alphabet_size; ++i){ //for each alphabet symbol
		if(num_elem[i] > 0){
			out_pos = in[in_pos];
			data->out_buf[out_pos++] = (uint8_t)i;
			for(int j = 1; j < num_elem[i]; ++j){ //for each occurrence of that symbol
				tmp = in[in_pos + j];
				if(tmp > 0){
					while(tmp > 0){
						if(data->out_buf[out_pos++] > i){
							tmp--;
						}
					}
				}

				while(data->out_buf[out_pos] < i)
					out_pos++;

				data->out_buf[out_pos++] = i;
			}
			in_pos += num_elem[i];
		}
	}

}


void IF::setNum_elem(const uint32_t *start){
	memcpy(num_elem, start, sizeof(int) * alphabet_size);
}

void IF::init(bool vector_init){
	memset(num_elem, 0, sizeof(uint32_t) * alphabet_size);
	memset(actual_dist, 0, sizeof(int) * alphabet_size);

	if(vector_init)
		for(int i = 0; i < alphabet_size; ++i)
			ret_value.push_back(vector<uint32_t>());
}

uint32_t *IF::getNum_elem() const{
	uint32_t *tmp =  new uint32_t[alphabet_size];
	memcpy(tmp, num_elem, sizeof(uint32_t) * alphabet_size);

	return tmp;
}

void IF::modifyTempElements(uint8_t elem){
	actual_dist[elem] = 0;
	for(int i = 0; i < elem; ++i){
		actual_dist[i]++;
	}
}

}
