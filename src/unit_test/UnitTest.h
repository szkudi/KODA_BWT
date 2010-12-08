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


	void testDCEncode(DC& dc);
	void testDCDecode(DC& dc);

	void testMTFEncode(MTF& mtf);
	void testMTFDecode(MTF& mtf);

	void testIFEncode(IF& mtf);
	void testIFDecode(IF& mtf);

	void testIFCEncode(IFC& mtf);
	void testIFCDecode(IFC& mtf);

	void testRLE_2Encode(RLE_2& mtf);
	void testRLE_2Decode(RLE_2& mtf);

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
