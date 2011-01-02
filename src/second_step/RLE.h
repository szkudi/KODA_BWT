/*!
 * \file RLE.h
 * \brief Implementation of RLE algorithm
 *
 *  \date 09-12-2010
 *  \author Kacper Szkudlarek
 */

#ifndef RLE_H_
#define RLE_H_

#include <stdint.h>
#include "SecondStepAlg.h"

namespace SecondStepAlgs {

class RLE {
public:
	/*!
	 * Constructor
	 */
	RLE();
	/*!
	 * Destructor
	 */
	virtual ~RLE();

	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Return length of encoded data. Encoded data is returned via pointer given to method
	 */
	int encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void encodeBuf(coderData* data);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param in_buf - pointer to data, which should be decode
	 * \param out_buf - pointer where decoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
	void decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void decodeBuf(coderData* data);

	int getResizeLength(const uint8_t* in_buf, int buf_size);
};

}

#endif /* RLE_H_ */
