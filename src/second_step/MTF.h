/*!
 * \file MTF.h
 * \brief Implementation of BWT second step algorithm - Move To Front
 *
 *  \date 29-11-2010
 *  \author Kacper Szkudlarek
 */

#ifndef MTF_H_
#define MTF_H_

#include "SecondStepAlg.h"

namespace SecondStepAlgs {

/*!
 * Implementation of BWT second step algorithm - Move To Front
 */

class MTF{// : public SecondStepAlg {
public:
	/*!
	 * Class constructor
	 */
	MTF();
	/*!
	 * Class destructor
	 */
	virtual ~MTF();

	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Encoded data is returned via pointer given to method
	 */
	void encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
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
private:
	/*!
	 * Method used to generate starting alphabet
	 */
	void init();

	static const int alphabet_size = 256;

	///Array containing used alphabet - ASCII
	uint8_t alphabet[alphabet_size];
};

}

#endif /* MTF_H_ */
