/*
 * ArthmeticCoder.cpp
 *
 *  Created on: 02-01-2011
 *      Author: szkudi
 */

#include "ArthmeticCoder.h"
#include <iostream>
#include <cmath>

using namespace std;

namespace SecondStepAlgs {

ArthmeticCoder::ArthmeticCoder() {
}

ArthmeticCoder::~ArthmeticCoder() {
}

void ArthmeticCoder::encodeBuf(coderData *data){

	uint64_t przedzial;
	int idx;
	int bits = 0;
	maska = 0x80;
	buf_pos = 2;

	init();

	countWeights(data->in_buf, data->in_size);


	data->out_buf = new uint8_t[data->in_size + alphabet.size() + 2 + (2 + alphabet.size()) * sizeof(uint32_t)];

	uint8_t* out = data->out_buf + 2 * sizeof(uint32_t);

	for(unsigned int i = 0; i < alphabet.size(); ++i){
		out[buf_pos++] = alphabet[i].c;
		(reinterpret_cast<uint32_t*>(out + buf_pos))[0] = alphabet[i].weight;
		buf_pos += sizeof(uint32_t);
	}
	out[1] = alphabet.size();

	for(unsigned int i = 0; i < data->in_size; ++i){
		idx = findElem(data->in_buf[i]);

		przedzial = (uint64_t)(gora - dol) + 1;
		dol = dol + przedzial*alphabet[idx].dol;
		gora = dol + przedzial*alphabet[idx].gora;

		cout << i << " gora = " << gora << " dol = " << dol << " przedzial = " << przedzial << " idx = " << idx << endl;

		while(true){
			if((gora & 0x80000000) == (dol & 0x80000000)){
				cout << "wysuwam" << endl;
				sendBit(data->out_buf, gora & 0x80000000);
				bits++;

				while(niedomiar > 0){
					cout << "niedomiar" << endl;
					sendBit(out, ~(gora & 0x80000000));
					bits++;
					niedomiar--;
				}
			}else if(!(gora & 0x80000000) && (dol & 0x80000000)){
				niedomiar++;
				dol &= 0x3fffffff;
				gora |= 0x40000000;
			}else
				break;

			dol <<= 1;
			gora <<= 1;
			gora |= 1;
		}
	}

	data->out_size = buf_pos + 1 + 2 * sizeof(uint32_t);

	cout << buf_pos;

	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size;
	(reinterpret_cast<uint32_t*>(data->out_buf))[1] = dol;
	out[0] = bits;

}

void ArthmeticCoder::decodeBuf(coderData *data){

	uint64_t przedzial;
	double l_kod;
	uint32_t kod;
	int bits;
	int count;
	int tmp;
	int idx;

	init();

	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];
	data->out_buf = new uint8_t[data->out_size];

	kod = (reinterpret_cast<uint32_t*>(data->in_buf))[1];

	uint8_t* in = data->in_buf + 2 * sizeof(uint32_t);

	buf_pos = 0;
	bits = in[buf_pos++];
	count = in[buf_pos++];

	for(int i = 0; i < count; ++i){
		tmp = in[buf_pos++];
		alphabet_weights[tmp].weight = (reinterpret_cast<uint32_t*>(in + buf_pos))[0];
		buf_pos += sizeof(uint32_t);
	}

	setupLimits(1);

	buf_pos += ceil(1.0*bits/8);
	maska = 1 << (7 - bits % 8);

	while((kod & 0x80) == 0){
		kod <<= 1;
		kod |= getBit(in);
	}

	for(unsigned int i = 0; i < data->out_size; ++i){
		przedzial = (uint64_t)(gora - dol) + 1;
		l_kod = 1.0*(kod - dol)/przedzial;

		idx = findElemByProbability(l_kod);

		data->out_buf[i] = alphabet[idx].c;

		przedzial = (uint64_t)(gora - dol) + 1;
		dol = dol + przedzial*alphabet[idx].dol;
		gora = dol + przedzial*alphabet[idx].gora;

		while(true){
			if((gora & 0x80000000) == (dol & 0x80000000)){
			}else if(!(gora & 0x80000000) && (dol & 0x80000000)){
				kod ^= 0x40000000;
				dol &= 0x3fffffff;
				gora |= 0x40000000;
			}else
				break;

			dol <<= 1;
			gora <<= 1;
			gora |= 1;
			kod <<= 1;
			kod += getBit(in);
		}
	}

}

int ArthmeticCoder::getBit(uint8_t* buffer){
	int ret;

	if((buffer[buf_pos] & maska) == 0)
		ret = 0;
	else
		ret = 1;

	if(maska != 0x80){
		maska <<= 1;
	}else{
		buf_pos--;
		maska = 1;
	}

	return ret;
}

void ArthmeticCoder::sendBit(uint8_t* buffer, uint32_t bit){
	if(bit > 0){
		buffer[buf_pos] |= maska;
	}
	if(maska != 1){
		maska >>= 1;
	}else{
		maska = 0x80;
		buf_pos++;
	}
}

void ArthmeticCoder::init(){
	gora = 0xFFFFFFFF;
	dol = 0;
	niedomiar = 0;

	for(int i = 0; i < alphabet_size; ++i){
		alphabet_weights[i].weight = 0;
		alphabet_weights[i].gora = 0.0;
		alphabet_weights[i].dol = 0.0;
	}
}

void ArthmeticCoder::countWeights(const uint8_t* in_buf, int buf_size){
	for(int i = 0; i < buf_size; ++i){
		alphabet_weights[in_buf[i]].weight++;
	}

	setupLimits(buf_size);
}

void ArthmeticCoder::setupLimits(int buf_size){
	bool first = true;

	for(int i = 1; i < alphabet_size; ++i){
		if(alphabet_weights[i].weight > 0){
			if(first){
				alphabet_weights[i].gora = 1.0*alphabet_weights[i].weight/buf_size;
				first = false;
			}else{
				alphabet_weights[i].dol = alphabet.back().gora;
				alphabet_weights[i].gora = alphabet_weights[i].dol + 1.0*alphabet_weights[i].weight/buf_size;
			}
			alphabet_weights[i].c = i;
			alphabet.push_back(alphabet_weights[i]);
		}
	}
}

int ArthmeticCoder::findElem(uint8_t c){
	for(unsigned int i = 0; i < alphabet.size(); ++i){
		if(alphabet[i].c == c)
			return i;
	}
	return -1;
}

int ArthmeticCoder::findElemByProbability(double prob){
	for(unsigned int i = 0; i < alphabet.size(); ++i){
		if(alphabet[i].gora > prob)
			return i;
	}
	return -1;
}

}//namespace


