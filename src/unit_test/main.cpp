/*
 * main.cpp
 *
 *  Created on: 02-12-2010
 *      Author: szkudi
 */

#include "UnitTest.h"

using namespace std;

int main(){

	UTest::UnitTest test;
	SecondStepAlgs::MTF mtf;
	SecondStepAlgs::DC dc;
	SecondStepAlgs::IF invfreq;
	SecondStepAlgs::RLE_2 rle_2;
	SecondStepAlgs::IFC ifc;

	test.testMTFEncode(mtf);

	test.testMTFDecode(mtf);

	test.testDCEncode(dc);

	test.testDCDecode(dc);

	test.testIFEncode(invfreq);

	test.testIFDecode(invfreq);

	test.testRLE_2Encode(rle_2);

	test.testRLE_2Decode(rle_2);

	test.testIFCEncode(ifc);

	test.testIFCDecode(ifc);

	return 0;

}
