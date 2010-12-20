/*!
 * \file DC.h
 * \brief Implementation of BWT second step algorithm - distance coder
 *
 *  \date 29-11-2010
 *  \author Kacper Szkudlarek
 */

#ifndef DC_H_
#define DC_H_

#include "SecondStepAlg.h"

namespace SecondStepAlgs {

class DC{//: public SecondStepAlgs::SecondStepAlg {
public:
	/*!
	 * Constructor
	 */
	DC();

	/*!
	 * Destrucotor
	 */
	virtual ~DC();

	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Encoded data is returned via pointer given to method
	 */
	int encodeBuf(const uint8_t* in_buf, uint32_t* out_buf, int buf_size);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param in_buf - pointer to data, which should be decode
	 * \param out_buf - pointer where decoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
	void decodeBuf(const uint32_t* in_buf, uint8_t* out_buf, int buf_size);

	/*!
	 * Getter method for alphabet start positions.
	 * \return pointer to copied data
	 */
	int* getStart_dst() const;
	/*!
	 * Setter method for alphabet start position
	 * \param start pointer to given data
	 */
    void setStart_dst(const int *start);


private:
	/*!
	 * Method used to generate starting alphabet
	 */
	void init();

	static const int alphabet_size = 256;

	///Array containing used alphabet - ASCII
	int start_pos[alphabet_size];
	int last_pos[alphabet_size];

};

}

#endif /* DC_H_ */
