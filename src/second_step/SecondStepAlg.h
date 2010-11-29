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

	virtual void encode_buf(uint8_t* buf, int buf_size) = 0;
	virtual void decode_buf(uint8_t* buf, int buf_size) = 0;


};

}//namespace SecondStepAlgs


#endif /* SECONDSTEPALG_H_ */
