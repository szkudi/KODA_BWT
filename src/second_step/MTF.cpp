/*
 * MTF.cpp
 *
 *  Created on: 29-11-2010
 *      Author: szkudi
 */

#include "MTF.h"
//#include <stdint.h>

namespace SecondStepAlgs {

using namespace std;

MTF::MTF():SecondStepAlg() {


}

MTF::~MTF() {

}

void MTF::reset_alphabet(){
	for(uint8_t i = 0; i < alphabet_size; ++i)
		alphabet[i] = i;
}

void MTF::encode_buf(uint8_t *buf, int buf_size){

	reset_alphabet();
	uint8_t tmp;

    for (int i = 0; i < buf_size; ++i) {
        tmp = alphabet[buf[i]];
        for (int j = 0; j < alphabet_size; ++j)
            if (alphabet[j] < alphabet[buf[i]])
                ++alphabet[j];
        alphabet[buf[i]] = 0;
        buf[i] = tmp;
    }
}

void MTF::decode_buf(uint8_t *buf, int buf_size){

    uint8_t tmp, buf_tmp;

    reset_alphabet();

    for (int i = 0; i < buf_size; ++i) {
        buf_tmp = alphabet[buf[i]];
        tmp = alphabet[buf[i]];
        for (int j = buf[i]; j ; --j)
        	alphabet[j] = alphabet[j-1];
        alphabet[0] = tmp;
        buf[i] = buf_tmp;
    }
}

}//namespace SecondStepAlgs
