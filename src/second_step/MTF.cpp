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

void MTF::encodeBuf(coderData* data){

	init();

	data->out_size = data->in_size + sizeof(uint32_t);
	data->out_buf = new uint8_t[data->out_size];

	uint8_t* out = data->out_buf + sizeof(uint32_t);

	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size;

    for (unsigned int i = 0; i < data->in_size; ++i) {
    	out[i] = alphabet[data->in_buf[i]];
        for (int j = 0; j < alphabet_size; ++j)
            if (alphabet[j] < alphabet[data->in_buf[i]])
                ++alphabet[j];
        alphabet[data->in_buf[i]] = 0;
    }
}

void MTF::decodeBuf(coderData* data){

    uint8_t tmp;

    init();

    data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];
    data->out_buf = new uint8_t[data->out_size];

    uint8_t* in = data->in_buf + sizeof(uint32_t);

    for (unsigned int i = 0; i < data->out_size; ++i) {
    	data->out_buf[i] = alphabet[in[i]];
        tmp = alphabet[in[i]];
        for (int j = in[i]; j ; --j)
        	alphabet[j] = alphabet[j-1];
        alphabet[0] = tmp;
    }
}

}//namespace SecondStepAlgs
