/*!
 * \file IF.h
 * \brief Implementation of BWT second step algorithm - inversion frequencies
 *
 *  \date 03-12-2010
 *  \author Kacper Szkudlarek
 */


#ifndef IF_H_
#define IF_H_

#include <stdint.h>
#include <vector>

using namespace std;

namespace SecondStepAlgs {

class IF {
public:
	/*!
	 * Constructor
	 */
	IF();
	/*!
	 * Destructor
	 */
	virtual ~IF();

	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Encoded data is returned via pointer given to method
	 */
	void encodeBuf(const uint8_t* in_buf, unsigned int* out_buf, int buf_size);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param in_buf - pointer to data, which should be decode
	 * \param out_buf - pointer where decoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
	void decodeBuf(const unsigned int* in_buf, uint8_t* out_buf, int buf_size);

	/*!
	 * Getter method for alphabet start positions.
	 * \return pointer to copied data
	 */
	int* getNum_elem() const;
	/*!
	 * Setter method for alphabet start position
	 * \param start pointer to given data
	 */
    void setNum_elem(const int *start);
private:
	/*!
	 * Method used to generate starting alphabet
	 */
	void init();
	/*!
	 * Method incrementing first elem element of actual_dist array
	 * \elem - number of elements to increment
	 */
	void modifyTempElements(uint8_t elem);

	static const int alphabet_size = 256;

	///Array containing used alphabet - ASCII
	int num_elem[alphabet_size];
	int actual_dist[alphabet_size];

	vector< vector<int> > ret_value;

};

}

#endif /* IF_H_ */
