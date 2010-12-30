/*
 * SecondStepAlg.h
 *
 *  Created on: 29-11-2010
 *      Author: szkudi
 */

#ifndef SECONDSTEPALG_H_
#define SECONDSTEPALG_H_

#include <stdint.h>

namespace SecondStepAlgs{

class SecondStepAlg{
public:
	virtual ~SecondStepAlg() {};

	virtual void encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size) = 0;
	virtual void decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size) = 0;

private:
	virtual void init() = 0;


};

typedef struct _coderData{
	uint8_t* in_buf;
	uint8_t* out_buf;

	int data_size;
	int ret_size;

}coderData;

}//namespace SecondStepAlgs


#endif /* SECONDSTEPALG_H_ */
