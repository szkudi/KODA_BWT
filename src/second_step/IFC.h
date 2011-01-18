/*!
 * \file IFC.h
 * \brief Implementation of Incremental Frequency Algorithm described in
 * "Incremental frequency count - a post BWT-stage for Burrows-Wheeler compression algorithm"
 * Jurgen Abel
 *
 *  \date 08-12-2010
 *  \author Kacper Szkudlarek
 */


#ifndef IFC_H_
#define IFC_H_

#include <stdint.h>
#include "SecondStepAlg.h"

namespace SecondStepAlgs {

class IFC {
public:
	/*!
	 * Constructor
	 */
	IFC(int window_size = 8, int dm = 16, int q = 64, int t = 256, double rb = 1.5);
	/*!
	 * Destructor
	 */
	virtual ~IFC();

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
	void init(bool lst_only = false);

	static const int alphabet_size = 256;

	typedef struct {
		uint8_t symbol;
		int index;
		double counter;
	} alphabeth_element;

	uint8_t alphabeth_list[alphabet_size];
	int alphabeth_index[alphabet_size];
	double alphabeth_counter[alphabet_size];

//	alphabeth_element alphabet[alphabet_size];

	//Algorithm parameters
	int window_size, dm, q, t, rb;
};


}

#endif /* IFC_H_ */
