/*
 * MTF.cpp
 *
 *  Created on: 29-11-2010
 *      Author: szkudi
 */

#include "MTF.h"
//#include <stdint.h>
#include <cstring>
#include <iostream>

namespace SecondStepAlgs {

using namespace std;

MTF::MTF(){//:SecondStepAlg() {


}

MTF::~MTF() {

}

void MTF::init(){
	for(int i = 0; i < alphabet_size; ++i)
		alphabet[i] = i;
}

void MTF::encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size){

	init();
//	cout << "Init" << endl;
//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

//	cout << "Start konwersji" << endl;

    for (int i = 0; i < buf_size; ++i) {
        out_buf[i] = alphabet[in_buf[i]];
        for (int j = 0; j < alphabet_size; ++j)
            if (alphabet[j] < alphabet[in_buf[i]])
                ++alphabet[j];
        alphabet[in_buf[i]] = 0;
    }
}

void MTF::decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size){

    uint8_t tmp;
//	out_buf = new uint8_t[buf_size];
//	memset(out_buf, 0, sizeof(uint8_t) * buf_size);

    init();

    for (int i = 0; i < buf_size; ++i) {
        out_buf[i] = alphabet[in_buf[i]];
        tmp = alphabet[in_buf[i]];
        for (int j = in_buf[i]; j ; --j)
        	alphabet[j] = alphabet[j-1];
        alphabet[0] = tmp;
    }
}

}//namespace SecondStepAlgs
