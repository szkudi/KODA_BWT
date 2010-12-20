/*!
 * \file UnitTest.cpp
 * \brief Unit tests implementation for each second step algorithm
 *
 *  \date 02-12-2010
 *  \author Kacper Szkudlarek
 */

#include "UnitTest.h"
#include <cstring>
#include <iostream>
#include <cstdio>
#include "IOData.h"

using namespace std;

namespace UTest{

UnitTest::UnitTest(){
}

UnitTest::~UnitTest(){
}

void UnitTest::testMTFEncode(MTF & mtf){
	cout << "Move To Front encoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	mtf.encodeBuf(input_data, ret, data_size);

	if(compareData(ret, output_mtf, data_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", input_data, data_size);
		writeOutputData("Retuned data: ", ret, data_size);
		writeOutputData("Should be: ", output_mtf, data_size);
	}

	delete[] ret;
}

void UnitTest::testMTFDecode(MTF & mtf){
	cout << "Move To Front decoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	mtf.decodeBuf(output_mtf, ret, data_size);

	if(compareData<uint8_t>(ret, input_data, data_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_mtf, data_size);
		writeOutputData("Retuned data: ", ret, data_size);
		writeOutputData("Should be: ", input_data, data_size);
	}

	delete[] ret;
}

void UnitTest::testDCEncode(DC & dc){
	cout << "Distance Coding encoding test: " << endl;

	unsigned int* ret = new unsigned int[2 * data_size];
	memset(ret, 0, sizeof(unsigned int) * 2 * data_size);
	dc.encodeBuf(input_data, ret, data_size);
//	int* dict = dc.getStart_dst();

	if(compareData<unsigned int>(ret, output_dc, data_size)){// && compareData<int>(dict, dict_dc, 256)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", input_data, data_size);
		writeOutputData("Retuned data: ", ret,  data_size + 9);
//		writeOutputData("Słownik: ",dict, 256);
		writeOutputData("Should be: ", output_dc, data_size + 9);
	}

	delete[] ret;
//	delete[] dict;
}

void UnitTest::testDCDecode(DC & dc){
	cout << "Distance Coding decoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
//	dc.setStart_dst(dict_dc);
	dc.decodeBuf(output_dc, ret, data_size);

	if(compareData<uint8_t>(ret, input_data, data_size)){
		cout << "Correct" << endl;
	}
	else{
		writeOutputData("Dane wejściowe: ", output_dc, data_size + 9);
		writeOutputData("Retuned data: ", ret, data_size, true);
		writeOutputData("Should be: ", input_data, data_size, true);
	}

	delete[] ret;
}

void UnitTest::testIFEncode(IF & invfreq){
	cout << "Inversion Frequencies encoding test: " << endl;

	uint32_t* ret = new uint32_t[2 * data_size];
	memset(ret, 0, sizeof(uint32_t) * 2 * data_size);
	invfreq.encodeBuf(if_input_data, ret, data_size);
//	int* num_elem = invfreq.getNum_elem();

	if(compareData(ret, output_if, data_size + 9)){// && compareData(num_elem, num_elem_if, 256)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", if_input_data, data_size);
		writeOutputData("Retuned data: ", ret, data_size + 9);
//		writeOutputData("Ilość elementów: ",num_elem, 256);
		writeOutputData("Should be: ", output_if, data_size);
	}

	delete[] ret;
//	delete[] num_elem;
}

void UnitTest::testIFDecode(IF & invfreq){
	cout << "Inversion Frequencies decoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	memset(ret, 0, sizeof(uint8_t) * data_size);
//	invfreq.setNum_elem(num_elem_if);
	invfreq.decodeBuf(output_if, ret, data_size);

	if(compareData(ret, if_input_data, data_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_if, data_size + 9);
		writeOutputData("Retuned data: ", ret, data_size, true);
//		writeOutputData("Should be: ", if_input_data, data_size, true);
	}

	delete[] ret;
}

void UnitTest::testRLE_2Encode(RLE_2 & rle){
	cout << "Run lenght encoding 2 encoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	int size = rle.encodeBuf(input_data, ret, data_size);
	uint32_t* buff = rle.getRLE_buffer();
	int rle_size = rle.getSizeRLE_buffer();

//	cout << size << endl;

	if(compareData(ret, output_rle_2, size) && compareData(buff, rle_buff, rle_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", input_data, data_size, true);
		writeOutputData("Retuned data: ", ret, size);
		writeOutputData("Bufor RLE: ",buff, rle_size);
		writeOutputData("Should be: ", output_rle_2, data_size, true);
	}

	delete[] ret;
	delete[] buff;
}

void UnitTest::testRLE_2Decode(RLE_2 & rle){
	cout << "Run length encoding 2 decoding test: " << endl;

	rle.setRLE_buffer(rle_buff, 3);
	uint8_t* ret = new uint8_t[7 + rle.getResizeValueRLE_buffer()];
	rle.decodeBuf(output_rle_2, ret, 7);

	if(compareData(ret, input_data, data_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_rle_2, 7, true);
		writeOutputData("Retuned data: ", ret, 7 + rle.getResizeValueRLE_buffer(), true);
		writeOutputData("Should be: ", input_data, data_size, true);
	}

	delete[] ret;
}

void UnitTest::testIFCEncode(IFC & ifc){
	cout << "Incremental Frequency Count encoding test: " << endl;

//	char tekst[] = "Toooo jjjeessst jjjaaakkkiissss ttttteeekkkkkkkkkkkstttttt";
//	int size = 58;
//
//	uint8_t* ret = new uint8_t[size];
//	uint8_t* ret2 = new uint8_t[size];
//	ifc.encodeBuf(reinterpret_cast<uint8_t*>(tekst), ret, size);
//
//	ifc.decodeBuf(ret, ret2, size);
//
//	writeOutputData("Dane wejściowe: ", tekst, size, true);
//	writeOutputData("Retuned data: ", ret, size);
//	writeOutputData("Ret2: ", ret2, size, true);


	uint8_t* ret = new uint8_t[7];
	ifc.encodeBuf(output_rle_2, ret, 7);

	if(compareData(ret, output_ifc, 7)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_rle_2, 7, true);
		writeOutputData("Retuned data: ", ret, 7);
		writeOutputData("Should be: ", output_ifc, 7);
	}

	delete[] ret;
//	delete[] ret2;
}

void UnitTest::testIFCDecode(IFC & ifc){
	cout << "Incremental Frequency Count decoding test: " << endl;

	uint8_t* ret = new uint8_t[7];
	ifc.decodeBuf(output_ifc, ret, 7);

	if(compareData(ret, output_rle_2, 7)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_ifc, 7);
		writeOutputData("Retuned data: ", ret, 7, true);
		writeOutputData("Should be: ", output_rle_2, 7, true);
	}

	delete[] ret;
}

void UnitTest::testRLEEncode(RLE& rle){
	cout << "Run Length Encoding encoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	int size = rle.encodeBuf(input_data, ret, data_size);

	if(compareData(ret, output_rle, size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", input_data, data_size, true);
		writeOutputData("Retuned data: ", ret, size);
		writeOutputData("Should be: ", output_ifc, 7);
	}

	delete[] ret;
}

void UnitTest::testRLEDecode(RLE & rle){
	cout << "Run Length Encoding decoding test: " << endl;

	int size = rle.getResizeLength(output_rle, 8);

	uint8_t* ret = new uint8_t[size];
	rle.decodeBuf(output_rle, ret, 8);

	if(compareData(ret, input_data, size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_rle, 8);
		writeOutputData("Retuned data: ", ret, size, true);
		writeOutputData("Should be: ", output_rle, 8, true);
	}

	delete[] ret;
}

void UnitTest::testHuffmanEncode(HuffmanCoder& hc){
	cout << "Huffman Coder encoding test: " << endl;

//	char tekst[] = "aaaabbbccd";

	uint8_t* ret = new uint8_t[2 * data_size];
	memset(ret, 0, sizeof(uint8_t) * 2 * data_size);
//	int size = hc.encodeBuf((uint8_t*)tekst, ret, data_size);
	int size = hc.encodeBuf(input_data, ret, data_size);

//	cout << size << endl;

	if(compareData(ret, output_huffman, ret[0] * 3 + size/8 + 2)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", input_data, 8);
		writeOutputData("Retuned data: ", ret, ret[0] * 3 + size/8 + 2);
		writeOutputData("Should be: ", output_huffman, 8, true);
	}

	delete[] ret;

}

void UnitTest::testHuffmanDecode(HuffmanCoder& hc){
	cout << "Huffman Coder decoding test: " << endl;

	uint8_t* ret = new uint8_t[data_size];
	hc.decodeBuf(output_huffman, ret, 20);

	if(compareData(ret, input_data, data_size)){
		cout << "Correct" << endl;
	}else{
		writeOutputData("Dane wejściowe: ", output_huffman, 14 + 20/8);
		writeOutputData("Retuned data: ", ret, data_size, true);
		writeOutputData("Should be: ", input_data, data_size, true);
	}


	delete[] ret;

}

template<typename T>
void UnitTest::writeOutputData(const char* str, const T* data, int data_size, bool character){
	printf("%s", str);
	for(int i = 0; i < data_size; ++i)
		if(character)
			printf("%c ", (int)(data[i]));
		else
			printf("0x%X ", (int)(data[i]));

	printf("\n");
}

template<typename T>
bool UnitTest::compareData(const T *first, const T *second, int data_size){
	int i = 0;
	while((i < data_size) && !(first[i] xor second[i])){
		i++;
	}
	return i == data_size;
}


}//namespace UTest
