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

class MTF : public SecondStepAlg {
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
	 * \param buf - pointer to data, which should be encode
	 * \param buf_size - size of data
	 *
	 * \return Encoded data is returned via pointer given to method
	 */
	void encode_buf(uint8_t* buf, int buf_size);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param buf - pointer to data, which should be encode
	 * \param buf_size - size of data
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
	void decode_buf(uint8_t* buf, int buf_size);
private:
	/*!
	 * Method used to generate starting alphabet
	 */
	void reset_alphabet();

	static const int alphabet_size = 256;

	///Array containing used alphabet - ASCII
	uint8_t alphabet[alphabet_size];
};

}

#endif /* MTF_H_ */
