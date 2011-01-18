/*
 * IFC.cpp
 *
 *  Created on: 08-12-2010
 *      Author: szkudi
 */

#include "IFC.h"
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

namespace SecondStepAlgs {

IFC::IFC(int window_size, int dm, int q, int t, double rb):window_size(window_size), dm(dm), q(q), t(t), rb(rb){
}

IFC::~IFC() {
}

void IFC::encodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	bool first = true;
	uint8_t last_symbol = 0xFF;
	double avg1 = 0.0;
	double inc1 = 16;
	double dif1;
	int lst_index_old;
	int index = 0;

	init();

	for(int i = 0 ;i < buf_size; ++i){
		//Calculate current index
		if(!first && in_buf[i] == last_symbol)
			index = alphabeth_index[last_symbol];
		else
			index = alphabeth_index[in_buf[i]];

		out_buf[i] = index;

		//Calculate difference from average rank
		dif1 = avg1;
		avg1 = ((avg1 * (window_size - 1)) + index) / window_size;
		dif1 = avg1 - dif1;

		//Calculate increment

		dif1 = min(fabs(dif1), (double)dm) * (dif1 < 0 ? -1 : 0);
		inc1 -= (inc1 * dif1)/q;

		//Check for run, increment counter
		if(!first && in_buf[i] == last_symbol)
			inc1 *= rb;
			//inc1 += inc1/2;

		last_symbol = in_buf[i];
		alphabeth_counter[in_buf[i]] += inc1;
		first = false;

		//Check for rescaling
		if(alphabeth_counter[in_buf[i]] > t){
			inc1 += 1.0;
			inc1 /= 2.0;
			for(int j = 0; j < alphabet_size; ++j){
				alphabeth_counter[j] += 1.0;
				alphabeth_counter[j] /= 2.0;
			}
		}

		//Sort list

		if(alphabeth_index[in_buf[i]]){
			uint8_t tmp_symbol = in_buf[i];
			lst_index_old =  alphabeth_index[in_buf[i]];
			while(lst_index_old-- > 0){
				if(alphabeth_counter[alphabeth_list[lst_index_old]] <= alphabeth_counter[tmp_symbol]){
					alphabeth_list[lst_index_old + 1] = alphabeth_list[lst_index_old];
					alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
				} else
					break;
			}
			alphabeth_list[lst_index_old + 1] = tmp_symbol;
			alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
		}

	}
}

void IFC::decodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){
	bool first = true;
	uint8_t last_symbol = 0xFF;
	double avg1 = 0.0;
	double inc1 = 16;
	double dif1;
	int lst_index_old;
	int index = 0;

	uint8_t symbol;

	init();

	for(int i = 0 ;i < buf_size; ++i){
		//Calculate current index
		if(!first && alphabeth_list[in_buf[i]] == last_symbol)
			index = alphabeth_index[last_symbol];
		else
			index = in_buf[i];

		symbol = out_buf[i] = alphabeth_list[index];

		//Calculate difference from average rank
		dif1 = avg1;
		avg1 = ((avg1 * (window_size - 1)) + index) / window_size;
		dif1 = avg1 - dif1;

		//Calculate increment

		dif1 = min(fabs(dif1), (double)dm) * (dif1 < 0 ? -1 : 0);
		inc1 -= (inc1 * dif1)/q;

		//Check for run, increment counter
		if(!first && symbol == last_symbol)
			inc1 *= rb;

		last_symbol = symbol;
		alphabeth_counter[symbol] += inc1;
		first = false;

		//Check for rescaling
		if(alphabeth_counter[symbol] > t){
			inc1 += 1.0;
			inc1 /= 2.0;
			for(int j = 0; j < alphabet_size; ++j){
				alphabeth_counter[j] += 1.0;
				alphabeth_counter[j] /= 2.0;
			}
		}

		//Sort list
		if(alphabeth_index[symbol]){
			uint8_t tmp_symbol = symbol;
			lst_index_old =  alphabeth_index[symbol];
			while(lst_index_old-- > 0){
				if(alphabeth_counter[alphabeth_list[lst_index_old]] <= alphabeth_counter[tmp_symbol]){
					alphabeth_list[lst_index_old + 1] = alphabeth_list[lst_index_old];
					alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
				} else
					break;
			}
			alphabeth_list[lst_index_old + 1] = tmp_symbol;
			alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
		}

	}
}

void IFC::encodeBuf(coderData* data){

	bool first = true;
	uint8_t last_symbol = 0xFF;
	double avg1 = 0.0;
	double inc1 = 16.0;
	double dif1;
	int lst_index_old;
	int index = 0;

	init();

	data->out_size = data->in_size + sizeof(uint32_t);
	data->out_buf = new uint8_t[data->out_size];
	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size;

	uint8_t* out = data->out_buf + sizeof(uint32_t);

	for(unsigned int i = 0; i < data->in_size; ++i){
		//Calculate current index
		if(!first && data->in_buf[i] == last_symbol)
			index = alphabeth_index[last_symbol];
		else
			index = alphabeth_index[data->in_buf[i]];

		out[i] = index;
		if(index > 255 || index < 0)
			std::cout << i << " " << index << endl;

		//Calculate difference from average rank
		dif1 = avg1;
		avg1 = ((avg1 * (window_size - 1)) + index) / window_size;
		dif1 = avg1 - dif1;

		//Calculate increment
		dif1 = min(fabs(dif1), (double)dm) * (dif1 < 0 ? -1 : 0);
		inc1 -= (inc1 * dif1)/q;

		//Check for run, increment counter
		if(!first && data->in_buf[i] == last_symbol)
			inc1 *= rb;

		last_symbol = data->in_buf[i];
		alphabeth_counter[data->in_buf[i]] += inc1;
		first = false;

		//Check for rescaling
		if(alphabeth_counter[data->in_buf[i]] > t){
			inc1 += 1.0;
			inc1 /= 2.0;
			for(int j = 0; j < alphabet_size; ++j){
				alphabeth_counter[j] += 1.0;
				alphabeth_counter[j] /= 2.0;
			}
		}

		//Sort list

		if(alphabeth_index[data->in_buf[i]]){
			uint8_t tmp_symbol = data->in_buf[i];
			lst_index_old =  alphabeth_index[data->in_buf[i]];
			while(lst_index_old-- > 0){
				if(alphabeth_counter[alphabeth_list[lst_index_old]] <= alphabeth_counter[tmp_symbol]){
					alphabeth_list[lst_index_old + 1] = alphabeth_list[lst_index_old];
					alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
				} else
					break;
			}
			alphabeth_list[lst_index_old + 1] = tmp_symbol;
			alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
		}

	}

}

void IFC::decodeBuf(coderData* data){
	bool first = true;
	uint8_t last_symbol = 0xFF;
	double avg1 = 0.0;
	double inc1 = 16;
	double dif1;
	int lst_index_old;
	int index = 0;

	uint8_t symbol;

	init();

	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];
	data->out_buf = new uint8_t[data->out_size];

	uint8_t* in = data->in_buf + sizeof(uint32_t);

	for(unsigned int i = 0 ;i < data->out_size; ++i){
		//Calculate current index
		if(!first && alphabeth_list[in[i]] == last_symbol)
			index = alphabeth_index[last_symbol];
		else
			index = in[i];

		symbol = data->out_buf[i] = alphabeth_list[index];

		//Calculate difference from average rank
		dif1 = avg1;
		avg1 = ((avg1 * (window_size - 1)) + index) / window_size;
		dif1 = avg1 - dif1;

		//Calculate increment

		dif1 = min(fabs(dif1), (double)dm) * (dif1 < 0 ? -1 : 0);
		inc1 -= (inc1 * dif1)/q;

		//Check for run, increment counter
		if(!first && symbol == last_symbol)
			inc1 *= rb;

		last_symbol = symbol;
		alphabeth_counter[symbol] += inc1;
		first = false;

		//Check for rescaling
		if(alphabeth_counter[symbol] > t){
			inc1 += 1.0;
			inc1 /= 2.0;
			for(int j = 0; j < alphabet_size; ++j){
				alphabeth_counter[j] += 1.0;
				alphabeth_counter[j] /= 2.0;
			}
		}

		//Sort list
		if(alphabeth_index[symbol]){
			uint8_t tmp_symbol = symbol;
			lst_index_old =  alphabeth_index[symbol];
			while(lst_index_old-- > 0){
				if(alphabeth_counter[alphabeth_list[lst_index_old]] <= alphabeth_counter[tmp_symbol]){
					alphabeth_list[lst_index_old + 1] = alphabeth_list[lst_index_old];
					alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
				} else
					break;
			}
			alphabeth_list[lst_index_old + 1] = tmp_symbol;
			alphabeth_index[alphabeth_list[lst_index_old + 1]] = lst_index_old + 1;
		}
	}
}

void IFC::init(bool lst_only){

	if(lst_only){
		for(int i = 0; i < alphabet_size; ++i)
			alphabeth_list[i] = i;
	} else{
		memset(alphabeth_counter, 0, sizeof(int) * alphabet_size);
		for(int i = 0; i < alphabet_size; ++i){
			alphabeth_list[i] = i;
			alphabeth_index[i] = i;
		}
	}
}

}
