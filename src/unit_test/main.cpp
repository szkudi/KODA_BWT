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

	test.testMTFEncode(mtf);

	test.testMTFDecode(mtf);

	test.testDCEncode(dc);

	test.testDCDecode(dc);

	test.testIFEncode(invfreq);

	test.testIFDecode(invfreq);

	return 0;

}
