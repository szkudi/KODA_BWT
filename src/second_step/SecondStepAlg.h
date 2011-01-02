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
	//Data buffers sizes
	uint32_t in_size;
	uint32_t out_size;

	//Data buffers
	uint8_t* in_buf;
	uint8_t* out_buf;

	//Additional variable for each coder use
	int var_1, var_2;
}coderData;

}//namespace SecondStepAlgs


#endif /* SECONDSTEPALG_H_ */
