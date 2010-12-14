/*!
 * \file UnitTest.h
 * \brief Unit tests definition for each second step algorithm
 *
 *  \date 02-12-2010
 *  \author Kacper Szkudlarek
 */


#ifndef UNITTEST_H_
#define UNITTEST_H_

#include <stdint.h>

#include "../second_step/DC.h"
#include "../second_step/MTF.h"
#include "../second_step/IF.h"
#include "../second_step/RLE_2.h"
#include "../second_step/IFC.h"
#include "../second_step/RLE.h"
#include "../second_step/HuffmanCoder.h"


using namespace std;
using namespace SecondStepAlgs;

namespace UTest{

class UnitTest {
public:
	/*!
	 * Constructor
	 */
	UnitTest();
	/*!
	 * Destructor
	 */
	~UnitTest();


	void testDCEncode(DC&);
	void testDCDecode(DC&);

	void testMTFEncode(MTF&);
	void testMTFDecode(MTF&);

	void testIFEncode(IF&);
	void testIFDecode(IF&);

	void testIFCEncode(IFC&);
	void testIFCDecode(IFC&);

	void testRLE_2Encode(RLE_2&);
	void testRLE_2Decode(RLE_2&);

	void testRLEEncode(RLE&);
	void testRLEDecode(RLE&);

	void testHuffmanEncode(HuffmanCoder&);
	void testHuffmanDecode(HuffmanCoder&);

private:
	template<typename T>
	void writeOutputData(const char* str, const T* data, int data_size, bool character = false);
	/*!
	 * Method to compare if two arrays of data are identical
	 * \param first pointer to first array to compare.
	 * \param second pointer to second array to compare.
	 * \param data_size size of comared data.
	 * \ret If data are the same 'True' otherwise 'False'
	 */
	template<typename T>
	bool compareData(const T *first, const T* second, int data_size);

	const static int data_size = 10;
};

}

//namespace UTest

#endif /* UNITTEST_H_ */
