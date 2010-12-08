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

using namespace std;

namespace SecondStepAlgs {

IFC::IFC(int window_size, int dm, int q, int t, double rb):window_size(window_size), dm(dm), q(q), t(t), rb(rb){

}

IFC::~IFC() {
	// TODO Auto-generated destructor stub
}

void IFC::encodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	bool first = true;
	uint8_t last_symbol = 0xFF;
	double avg1 = 0.0;
	int inc1 = 16;
	double dif1;
	int lst_index_old, lst_index_new;
	int index = 0;

	init();

	for(int i = 0 ;i < buf_size; ++i){
		//Calculate current index
		if(!first && in_buf[i] == last_symbol){
			index = 0;
		}else{
//			if(!first && alphabeth_index[in_buf[i]] > alphabeth_index[last_symbol]){
				index = alphabeth_index[in_buf[i]];
//			}else{
//				index = alphabeth_index[in_buf[i]] + 1;
//			}
		}
		out_buf[i] = index;

		//Calculate difference from average rank
		dif1 = avg1;
		avg1 = ((avg1 * (window_size - 1)) + index) / window_size;
		dif1 = avg1 - dif1;

		//Calculate increment

		dif1 = min(fabs(dif1), (double)dm) * (dif1 < 0 ? -1 : 0);
		inc1 -= (inc1 * dif1)/q;

//		if(dif1 >= 0.0){
//			if(dif1 > dm)
//				dif1 = dm;
//			inc1 -= ((inc1 * dif1) / q);
//		} else {
//			if(dif1 < -dm)
//				dif1 = -dm;
//			inc1 += ((inc1 * dif1) / q);
//		}

		//Check for run, increment counter
		if(!first && in_buf[i] == last_symbol)
			inc1 *= rb;
			//inc1 += inc1/2;

		last_symbol = in_buf[i];
		alphabeth_counter[in_buf[i]] += inc1;
		first = false;

		//Check for rescaling
		if(alphabeth_counter[in_buf[i]] > t){
			inc1 /= 2;
//			inc1 = (inc1 + 1) / 2;
			for(int j = 0; j < alphabet_size; ++j)
				alphabeth_counter[j] /= 2;
//				alphabeth_counter[j] = (alphabeth_counter[j] + 1) / 2;
		}

		//Sort list
//		uint8_t tmp_list;
//		int tmp_index;
//		lst_index_old = alphabeth_index[in_buf[i]];
//		while(lst_index_old > 1 && alphabeth_counter[alphabeth_list[lst_index_old - 1]] <= alphabeth_counter[alphabeth_list[lst_index_old]]){
//			tmp_list = alphabeth_list[lst_index_old];
//			tmp_index = alphabeth_index[alphabeth_list[lst_index_old]];
//			alphabeth_list[lst_index_old] = alphabeth_list[lst_index_old -1];
//			alphabeth_index[alphabeth_list[lst_index_old]] = alphabeth_index[alphabeth_list[lst_index_old - 1]];
//			lst_index_old--;
//			alphabeth_list[lst_index_old] = tmp_list;
//			alphabeth_index[alphabeth_list[lst_index_old]] = tmp_index;
//		}

		lst_index_old = alphabeth_index[in_buf[i]];
		if(lst_index_old > 0){
			lst_index_new = lst_index_old;
			while(lst_index_new > 0 && alphabeth_counter[alphabeth_list[lst_index_new - 1]] <= alphabeth_counter[alphabeth_list[lst_index_old]])
				lst_index_new--;
			if(lst_index_new < lst_index_old){
				//Move list
				for(int j = lst_index_old; j > lst_index_new+1; --j){ //lst_index_new+1
					alphabeth_list[j] = alphabeth_list[j - 1];
					alphabeth_index[alphabeth_list[j]] = j;
				}
				alphabeth_list[lst_index_new] = in_buf[i];
				alphabeth_index[in_buf[i]] = lst_index_new;
			}
		}
	}

}

void IFC::decodeBuf(const uint8_t *in_buf, uint8_t *out_buf, int buf_size){

	uint8_t tmp;

    init(true);

    for (int i = 0; i < buf_size; ++i) {
    	out_buf[i] = alphabeth_list[in_buf[i]];
        tmp = alphabeth_list[in_buf[i]];
        for (int j = in_buf[i]; j ; --j)
        	alphabeth_list[j] = alphabeth_list[j-1];
        alphabeth_list[0] = tmp;
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
