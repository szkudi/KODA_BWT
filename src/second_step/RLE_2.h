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
    int* getRLE_buffer() const;
    int getSizeRLE_buffer() const;
    int getResizeValueRLE_buffer() const;
    void setRLE_buffer(const int* buffer, int size);

private:
//	uint8_t run_symbol;
	int output_index;
	vector<int> RLE_buffer;
};

}

#endif /* RLE_2_H_ */
