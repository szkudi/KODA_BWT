/*
 * \file ArthmeticCoder.h
 *
 * \brief Implementation of arthmetic coder
 *
 *  \date 08-12-2010
 *  \author Kacper Szkudlarek
 */

#ifndef ARTHMETICCODER_H_
#define ARTHMETICCODER_H_

#include <vector>
#include <stdint.h>
#include "SecondStepAlg.h"

namespace SecondStepAlgs {

class ArthmeticCoder {
public:
	ArthmeticCoder();
	virtual ~ArthmeticCoder();
	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Return number of bits in output. Encoded data is returned via pointer given to method
	 */
//	int encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void encodeBuf(coderData* data);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param in_buf - pointer to data, which should be decode
	 * \param out_buf - pointer where decoded data will be returned
	 * \param buf_size - size of data (in bits)
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
//	int decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void decodeBuf(coderData* data);

private:
	static const int alphabet_size = 256;

	void init();
	/*!
	 * Count weight of each alphabet_weights symbol.
	 *
	 * \param in_buf input data buffer
	 * \param buf_size input buffer size
	 */
	void countWeights(const uint8_t* in_buf, int buf_size);
	/*!
	 * Method to find element in vector
	 */
	int findElem(uint8_t c);

	int findElemByProbability(double prob);

	/*!
	 * Writes one bit to output
	 */
	void sendBit(uint8_t* buffer, uint32_t bit);

	int getBit(uint8_t* buffer);

	void setupLimits(int buf_size);

	typedef struct _alphabetElement{
		uint32_t weight;
		double gora;
		double dol;

		uint8_t c;

	}alphabetElement;

	alphabetElement alphabet_weights[alphabet_size];

	std::vector<alphabetElement> alphabet;

	uint32_t gora;
	uint32_t dol;
	uint32_t niedomiar;

	uint8_t maska;
	int buf_pos;
};

}//namespace

#endif /* ARTHMETICCODER_H_ */
