/*!
 * \file RLE_2.h
 * \brief Implementation of RLE2 algorithm described in
 * "Incremental frequency count - a post BWT-stage for Burrows-Wheeler compression algorithm"
 * Jurgen Abel
 *
 *  \date 08-12-2010
 *  \author Kacper Szkudlarek
 */


#ifndef RLE_2_H_
#define RLE_2_H_

#include <stdint.h>
#include <vector>
#include "SecondStepAlg.h"

using namespace std;

namespace SecondStepAlgs {

class RLE_2 {
public:
	/*!
	 * Constructor
	 */
	RLE_2();
	/*!
	 * Destructor
	 */
	virtual ~RLE_2();

	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Return number of elements in encoded array. Encoded data is returned via pointer given to method
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
    /*!
     * Method returning vector containing runs lengths in array form
     *
     * \ret pointer to runs lengths array
     */
	uint32_t* getRLE_buffer() const;
	/*!
	 * Method returning size of runs length vector
	 *
	 * \return vector size
	 */
    int getSizeRLE_buffer() const;
    /*!
     * Method calculate how much longer will be decoded data
     *
     * \return extra data length
     */
    int getResizeValueRLE_buffer() const;
    /*!
     * Method to setup runs length vector
     *
     * \param buffer - pointer to data
     * \param size - data length
     */
    void setRLE_buffer(const uint32_t* buffer, int size);

private:
	int output_index;
	vector<uint32_t> RLE_buffer;
};

}

#endif /* RLE_2_H_ */
