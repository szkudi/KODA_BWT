/* 
 * File:   main.cpp
 * Author: JM
 *
 * Created on 22 listopad 2010, 17:59
 */

#include <iostream>
#include <fstream>
#include <stack>

#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include "bwt/BWT.h"

#include "second_step/DC.h"
#include "second_step/MTF.h"
#include "second_step/IF.h"
#include "second_step/RLE_2.h"
#include "second_step/IFC.h"
#include "second_step/RLE.h"
#include "second_step/HuffmanCoder.h"

using namespace std;
const static int alphabet_size = 256;


void printUsage(const char * exe) {
	printf( "USAGE:\n"
			"\t%s [options] <input_filename>\n",
			exe);
}

void printHelp(void) {
	cout << "Possible options: " << endl;
	cout << "d -- decoding data" << endl;
	cout << "e -- encoding data" << endl;
	cout << "h -- print this help" << endl;
	cout << "i <filename> -- input file" << endl;
	cout << "o <filename> -- output file" << endl;
	cout << "s <size> -- BWT block size" << endl;
	cout << "v -- verbose mode" << endl;
	cout << "1 -- RLE-2 + IFC + Huffman Coding" << endl;
	cout << "2 -- RLE-0 + IFC + Huffman Coding" << endl;
	cout << "3 -- IFC + Huffman Coding" << endl;
	cout << "4 -- DC + RLE + Huffman Coding" << endl;
	cout << "5 -- DC + Huffman Coding" << endl;
	cout << "6 -- IF + RLE + Huffman Coding" << endl;
	cout << "7 -- IF + Huffman Coding" << endl;
	cout << "8 -- MTF + RLE + Huffman Coding" << endl;
	cout << "9 -- MTF + Huffman Coding" << endl;

}

uint8_t* doMTFencode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::MTF mtf;

	cout << "MTF start" << endl;

	uint8_t* buf_2 = new uint8_t[data_size];
	memset(buf_2, 0 , sizeof(uint8_t) * data_size);
	mtf.encodeBuf(buf_1, buf_2, data_size);

	cout << "MTF end" << endl;

	return buf_2;
}

uint8_t* doMTFdecode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::MTF mtf;

	cout << "MTF start" << endl;

	uint8_t* buf_2 = new uint8_t[data_size];
	memset(buf_2, 0 , sizeof(uint8_t) * data_size);

	mtf.decodeBuf(buf_1, buf_2, data_size);

	cout << "MTF end" << endl;

	return buf_2;
}

uint8_t* doHCencode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::HuffmanCoder hc;

	cout << "HC start" << endl;

	int size2 = data_size + alphabet_size + 2 * sizeof(int32_t);
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0 , sizeof(uint8_t) * size2);

	int32_t bits = hc.encodeBuf(buf_1, buf_2 + 2 * sizeof(int32_t), data_size);

	int real_data_size = hc.getData_size();
	real_data_size += 2 * sizeof(uint32_t);

	(reinterpret_cast<int32_t*>(buf_2))[0] = data_size;
	(reinterpret_cast<int32_t*>(buf_2))[1] = bits;

	cout << "HC end" << endl;

	return buf_2;
}

uint8_t* doHCdecode(uint8_t* buf_1){
	SecondStepAlgs::HuffmanCoder hc;

	cout << "HC start" << endl;

	int size2 = (reinterpret_cast<int32_t*>(buf_1))[0];
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0 , sizeof(uint8_t) * size2);

	int32_t bits = (reinterpret_cast<int32_t*>(buf_1))[1];

	hc.decodeBuf(buf_1 + sizeof(uint32_t) * 2, buf_2, bits);

	cout << "HC end" << endl;

	return buf_2;
}

uint8_t* doRLEencode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::RLE rle;

	cout << "RLE start" << endl;

	int size2 = 2 * data_size + sizeof(uint32_t);
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0, sizeof(uint8_t) * size2);

	(reinterpret_cast<uint32_t*>(buf_2))[0] = data_size;
	rle.encodeBuf(buf_1, buf_2 + sizeof(uint32_t), data_size);

	cout << "RLE end" << endl;

	return buf_2;
}

uint8_t* doRLEdecode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::RLE rle;

	cout << "RLE start" << endl;

	int size2 = (reinterpret_cast<uint32_t*>(buf_1))[0];
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0, sizeof(uint8_t) * size2);

	rle.decodeBuf(buf_1 + sizeof(uint32_t), buf_2, data_size - sizeof(uint32_t));

	cout << "RLE end" << endl;

	return buf_2;
}

uint8_t* doDCencode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::DC dc;

	cout << "DC start" << endl;

	int size2 = data_size + alphabet_size + 1;
	uint32_t* buf_3 = new uint32_t[size2];
	memset(buf_3, 0 , sizeof(uint32_t) * size2);

	buf_3[0] = data_size;

	dc.encodeBuf(buf_1, buf_3 + 1, data_size);

	cout << "DC end" << endl;

	return reinterpret_cast<uint8_t*>(buf_3);

}

uint8_t* doDCdecode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::DC dc;

	cout << "DC start" << endl;

	uint32_t* buf_3 = reinterpret_cast<uint32_t*>(buf_1);

	int size2 = buf_3[0];

	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0 , sizeof(uint8_t) * size2);

	dc.decodeBuf(buf_3 + 1, buf_2, size2);

	cout << "DC end" << endl;

	return buf_2;
}

uint8_t* doIFCencode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::IFC ifc;

	cout << "IFC start" << endl;

	uint8_t* buf_2 = new uint8_t[data_size];
	memset(buf_2, 0 , sizeof(uint8_t) * data_size);

	ifc.encodeBuf(buf_1, buf_2, data_size);

	cout << "IFC end" << endl;

	return buf_2;
}

uint8_t* doIFCdecode(uint8_t* buf_1, int data_size){
	SecondStepAlgs::IFC ifc;

	cout << "IFC start" << endl;

	uint8_t* buf_2 = new uint8_t[data_size];
	memset(buf_2, 0 , sizeof(uint8_t) * data_size);

	ifc.decodeBuf(buf_1, buf_2, data_size);

	cout << "IFC end" << endl;

	return buf_2;
}

uint8_t* doRLE2andIFCandHCencode(uint8_t* buf_1, int data_size ){
	SecondStepAlgs::RLE_2 rle2;

	cout << "RLE2 start" << endl;

	int size2 = data_size + (data_size + 2) * sizeof(uint32_t);
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0, sizeof(uint8_t) * size2);

	(reinterpret_cast<uint32_t*>(buf_2))[0] = data_size;
	(reinterpret_cast<uint32_t*>(buf_2))[1] = rle2.encodeBuf(buf_1, buf_2 + sizeof(uint32_t) * 3, data_size);

	int real_data_size = (reinterpret_cast<uint32_t*>(buf_2))[1] + (reinterpret_cast<uint32_t*>(buf_2))[2] * sizeof(uint32_t) + 2 * sizeof(uint32_t);

	uint32_t * runs = rle2.getRLE_buffer();

	uint8_t* buf_3 = doIFCencode(buf_2, real_data_size);

	delete[] buf_2;
	buf_2 = new uint8_t[rle2.getSizeRLE_buffer() * sizeof(uint32_t) + real_data_size + 2 * sizeof(uint32_t)];

	memcpy(buf_2 + 2 * sizeof(uint32_t), runs, rle2.getSizeRLE_buffer() * sizeof(uint32_t));
	memcpy(buf_2 + 2 * sizeof(uint32_t) + rle2.getSizeRLE_buffer() * sizeof(uint32_t), buf_3, real_data_size);
	(reinterpret_cast<uint32_t*>(buf_2))[0] =  rle2.getSizeRLE_buffer();
	(reinterpret_cast<uint32_t*>(buf_2))[0] =  real_data_size;

	delete[] buf_3;
	buf_3 = doHCencode(buf_2, rle2.getSizeRLE_buffer() * sizeof(uint32_t) + real_data_size);

	cout << "RLE2 end" << endl;

	return buf_3;
}

uint8_t* doRLE2andIFCandHCdecode(uint8_t* buf_1, int data_size ){
	SecondStepAlgs::RLE_2 rle2;

	cout << "RLE 2 start" << endl;

	uint8_t* buf = doHCdecode(buf_1);

	int run_size = (reinterpret_cast<uint32_t*>(buf))[0];
	int real_data_size = (reinterpret_cast<uint32_t*>(buf))[0];

	uint32_t* runs = new uint32_t[run_size];
	memcpy(runs, buf + 2 * sizeof(uint32_t), run_size * sizeof(uint32_t));

	uint8_t* buf_3 = new uint8_t[real_data_size];
	memcpy(buf_3, buf + 2 * sizeof(uint32_t) + run_size * sizeof(uint32_t), real_data_size);

	delete[] buf;

	buf = doIFCdecode(buf_3, real_data_size);

	int size2 = (reinterpret_cast<uint32_t*>(buf))[0];
	uint8_t* buf_2 = new uint8_t[size2];
	memset(buf_2, 0, sizeof(uint8_t) * size2);

	rle2.setRLE_buffer(runs, run_size);
	rle2.encodeBuf(buf  + sizeof(uint32_t) * 2 , buf_2, size2);

	cout << "RLE2 end" << endl;

	return buf_2;
}

/*
 * Entry point for the project
 */
int main(int argc, char** argv) {

	char* in_filename = NULL;
	char* out_filename = NULL;
	fstream input;
	fstream output;
	int block_size = 0;
	bool verbose = false;
	bool decoding = false, encoding = false;
	int alg_type = 0;
	int data_size = 0;
	int output_size = 0;
	uint8_t *buf_1;
	uint8_t *buf_2;

//	BWTcoder *encoder;
//	BWTdecoder *decoder;

	if (argc < 2) {
		printUsage(argv[0]);
		return EXIT_SUCCESS;
	}

	// Parsing console parameters
	int c;
	while ( (c = getopt(argc, argv, "dehi:o:s:v123456789")) != -1) {
		switch (c) {
		case 'd':
			decoding = true;
			break;
		case 'e':
			encoding = true;
			break;
		case 'i':
			in_filename = optarg;
			break;
		case 'o':
			out_filename = optarg;
			break;
		case 's':
			block_size = atoi(optarg);
			break;
		case 'v':
			verbose = true;
			break;
		case '1':
//			cout << "RLE-2 + IFC + Huffman Coding" << endl;
			alg_type = 1;
			break;
		case '2':
//			cout << "RLE-0 + IFC + Huffman Coding" << endl;
			alg_type = 2;
			break;
		case '3':
//			cout << "IFC + Huffman Coding" << endl;
			alg_type = 3;
			break;
		case '4':
//			cout << "DC + RLE + Huffman Coding" << endl;
			alg_type = 4;
			break;
		case '5':
//			cout << "DC + Huffman Coding" << endl;
			alg_type = 5;
			break;
		case '6':
//			cout << "IF + RLE + Huffman Coding" << endl;
			alg_type = 6;
			break;
		case '7':
//			cout << "IF + Huffman Coding" << endl;
			alg_type = 7;
			break;
		case '8':
//			cout << "MTF + RLE + Huffman Coding" << endl;
			alg_type = 8;
			break;
		case '9':
//			cout << "MTF + Huffman Coding" << endl;
			alg_type = 9;
			break;
		case '?':
		case 'h':
			printHelp();
			return EXIT_SUCCESS;
		default:
			printf("Unknown argument: %c!\n", c);
			return EXIT_FAILURE;
			break;
		}
	}
	if (optind < argc) {
		printf("Too many input files specified!\n");
		return EXIT_FAILURE;
	}

	//Checking if all required parameters are set up

	input.open(in_filename, fstream::in|fstream::binary);
	if(input.fail()){
		cout << "Couldn't open input file: " << in_filename << endl;
		return EXIT_FAILURE;
	} else{
		input.seekg(0, fstream::end);
		data_size = input.tellg();
		if(data_size == -1){
			cout << "Couldn't get input file size: " << in_filename << endl;
			return EXIT_FAILURE;
		}
		input.seekg(0, fstream::beg);
	}

	output.open(out_filename, fstream::out|fstream::trunc|fstream::binary);
	if(output.fail()){
		cout << "Couldn't open output file: " << out_filename << endl;
		return EXIT_FAILURE;
	}

	if(alg_type == 0){
		cout << "You have to choose working mode" << endl;
		printHelp();
		return EXIT_FAILURE;
	}

	if((!encoding && !decoding) || (decoding && encoding)){
		cout <<"You must choose one working mode" << endl;
		printHelp();
		return EXIT_FAILURE;
	}

	if(block_size == 0){
		block_size = 10600; //Using standard size
	}

	if(verbose){
		cout << "Program call summary: " << endl;
		cout << "Input filename = " << in_filename << endl;
		cout << "Input data size = " << data_size << endl;
		cout << "Output filename = " << out_filename << endl;
		cout << "Algorithm type: ";
		switch(alg_type){
			case 1:
				cout << "RLE-2 + IFC + Huffman Coding" << endl;
				break;
			case 2:
				cout << "RLE-0 + IFC + Huffman Coding" << endl;
				break;
			case 3:
				cout << "IFC + Huffman Coding" << endl;
				break;
			case 4:
				cout << "DC + RLE + Huffman Coding" << endl;
				break;
			case 5:
				cout << "DC + Huffman Coding" << endl;
				break;
			case 6:
				cout << "IF + RLE + Huffman Coding" << endl;
				break;
			case 7:
				cout << "IF + Huffman Coding" << endl;
				break;
			case 8:
				cout << "MTF + RLE + Huffman Coding" << endl;
				break;
			case 9:
				cout << "MTF + Huffman Coding" << endl;
				break;
		}
		if(block_size == 10600)
			cout << "Using standard block size = " << block_size << endl;
		else
			cout << "Block size = " << block_size << endl;
	}

	//Executing encoding/decoding


	if(encoding){
		buf_1 = new uint8_t[data_size];
		input.read(reinterpret_cast<char *>(buf_1), data_size);

		switch(alg_type){
			case 1:
				// "RLE-2 + IFC + Huffman Coding"
				buf_2 = doRLE2andIFCandHCencode(buf_1, data_size);
				break;
			case 2:
				// "RLE-0 + IFC + Huffman Coding"
				buf_2 = doRLEencode(buf_1, data_size);

				break;
			case 3:
				cout << "IFC + Huffman Coding" << endl;
				break;
			case 4:
				cout << "DC + RLE + Huffman Coding" << endl;
				break;
			case 5:
				cout << "DC + Huffman Coding" << endl;
				break;
			case 6:
				cout << "IF + RLE + Huffman Coding" << endl;
				break;
			case 7:
				cout << "IF + Huffman Coding" << endl;
				break;
			case 8:
				cout << "MTF + RLE + Huffman Coding" << endl;
				break;
			case 9:
				cout << "MTF + Huffman Coding" << endl;
				break;
		}

		delete[] buf_1;
	}

	if(decoding){
		switch(alg_type){
			case 1:
				cout << "RLE-2 + IFC + Huffman Coding" << endl;
				break;
			case 2:
				cout << "RLE-0 + IFC + Huffman Coding" << endl;
				break;
			case 3:
				cout << "IFC + Huffman Coding" << endl;
				break;
			case 4:
				cout << "DC + RLE + Huffman Coding" << endl;
				break;
			case 5:
				cout << "DC + Huffman Coding" << endl;
				break;
			case 6:
				cout << "IF + RLE + Huffman Coding" << endl;
				break;
			case 7:
				cout << "IF + Huffman Coding" << endl;
				break;
			case 8:
				cout << "MTF + RLE + Huffman Coding" << endl;
				break;
			case 9:
				cout << "MTF + Huffman Coding" << endl;
				break;
		}
	}

	return EXIT_SUCCESS;
}

//***************************************
//ENCODING
//***************************************
//		input.open(in_filename, fstream::in);
//		if(input.fail()){
//			cout << "Couldn't open input file: " << in_filename << endl;
//			return EXIT_FAILURE;
//		} else{
//			input.seekg(0, fstream::end);
//			data_size = input.tellg();
//			if(data_size == -1){
//				cout << "Couldn't get input file size: " << in_filename << endl;
//				return EXIT_FAILURE;
//			}
//			input.seekg(0, fstream::beg);
//		}
//
//BWTcoder encoder(block_size);
//
//cout << "BWT start" << endl;
//
//int size2 = data_size + ceil(data_size/block_size)*sizeof(int) + sizeof(uint32_t);
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0 , sizeof(uint8_t) * size2);
//(reinterpret_cast<uint32_t*>(buf_2))[0] = data_size;
//encoder.code(reinterpret_cast<unsigned char*>(buf_1), buf_2 + sizeof(uint32_t), data_size);
//
//output.open("encoded.bwt", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//delete[] buf_2;
//
//cout << "BWT end" << endl;
//
//SecondStepAlgs::MTF mtf;
//
//cout << "MTF start" << endl;
//
//buf_2 = new uint8_t[data_size];
//memset(buf_2, 0 , sizeof(uint8_t) * data_size);
//mtf.encodeBuf(buf_1, buf_2, data_size);
//
//output.open("encoded.mtf", fstream::out|fstream::trunc);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * data_size);
//output.close();
//
//delete[] buf_2;
//
//cout << "MTF end" << endl;
//
//SecondStepAlgs::HuffmanCoder hc;
//
//cout << "HC start" << endl;
//
//size2 = data_size + alphabet_size + 2 * sizeof(int32_t);
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0 , sizeof(uint8_t) * size2);
//
//int32_t bits = hc.encodeBuf(buf_1, buf_2 + 2 * sizeof(int32_t), data_size);
//
//int real_data_size = hc.getData_size();
//real_data_size += 2 * sizeof(uint32_t);
//
//(reinterpret_cast<int32_t*>(buf_2))[0] = data_size;
//(reinterpret_cast<int32_t*>(buf_2))[1] = bits;
////		size2 = ceil(bits/8) + 3 * buf_2[sizeof(int32_t) * 2] + 2 * sizeof(int32_t);
//
////		cout << "Real size = " << real_data_size << " Size2 = " << size2 << endl;
//
////		cout << "Bits = " << bits << " " << (reinterpret_cast<int32_t*>(buf_2))[1] << endl;
//
//output.open("encoded.hc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * real_data_size);
//output.close();
//
//delete[] buf_2;
//
//cout << "HC end" << endl;
//
//SecondStepAlgs::DC dc;
//
//cout << "DC start" << endl;
//
//size2 = data_size + alphabet_size + 1;
//uint32_t* buf_3 = new uint32_t[size2];
//memset(buf_3, 0 , sizeof(uint32_t) * size2);
//
//buf_3[0] = data_size;
//
//real_data_size = dc.encodeBuf(buf_1, buf_3 + 1, data_size);
//real_data_size ++;
//
//output.open("encoded.dc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_3), sizeof(uint32_t) * real_data_size);
//output.close();
//
//delete[] buf_3;
//
//cout << "DC end" << endl;
//
//SecondStepAlgs::IF invfreq;
//
//cout << "IF start" << endl;
//
//size2 = data_size + 2 * alphabet_size + 1;
//buf_3 = new uint32_t[size2];
//memset(buf_3, 0 , sizeof(uint32_t) * size2);
//
//buf_3[0] = data_size;
//
//real_data_size = invfreq.encodeBuf(buf_1, buf_3 + 1, data_size);
//real_data_size ++;
//
//output.open("encoded.if", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_3), sizeof(uint32_t) * real_data_size);
//output.close();
//
//delete[] buf_3;
//
//cout << "IF end" << endl;
//
//SecondStepAlgs::IFC ifc;
//
//cout << "IFC start" << endl;
//
//buf_2 = new uint8_t[data_size];
//memset(buf_2, 0 , sizeof(uint8_t) * data_size);
//
//ifc.encodeBuf(buf_1, buf_2, data_size);
//
//output.open("encoded.ifc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * data_size);
//output.close();
//
//delete[] buf_2;
//
//cout << "IFC end" << endl;
//
//SecondStepAlgs::RLE rle;
//
//cout << "RLE start" << endl;
//
//size2 = 2 * data_size + sizeof(uint32_t);
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0, sizeof(uint8_t) * size2);
//
//(reinterpret_cast<uint32_t*>(buf_2))[0] = data_size;
//real_data_size = rle.encodeBuf(buf_1, buf_2 + sizeof(uint32_t), data_size);
//real_data_size += sizeof(uint32_t);
//
//output.open("encoded.rle", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * real_data_size);
//output.close();
//
//delete[] buf_2;
//
//cout << "RLE end" << endl;
//
//SecondStepAlgs::RLE_2 rle2;
//
//cout << "RLE start" << endl;
//
//size2 = data_size + (data_size + 3) * sizeof(uint32_t);
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0, sizeof(uint8_t) * size2);
//
//(reinterpret_cast<uint32_t*>(buf_2))[0] = data_size;
//(reinterpret_cast<uint32_t*>(buf_2))[1] = rle2.encodeBuf(buf_1, buf_2 + sizeof(uint32_t) * 3, data_size);
//(reinterpret_cast<uint32_t*>(buf_2))[2] = rle2.getSizeRLE_buffer();
//
//real_data_size = (reinterpret_cast<uint32_t*>(buf_2))[1] + (reinterpret_cast<uint32_t*>(buf_2))[2] * sizeof(uint32_t) + 3 * sizeof(uint32_t);
//
//uint32_t * tmp = rle2.getRLE_buffer();
//memcpy(buf_2 + 3 * sizeof(uint32_t) + (reinterpret_cast<uint32_t*>(buf_2))[1] * sizeof(uint8_t), tmp, (reinterpret_cast<uint32_t*>(buf_2))[2] * sizeof(uint32_t));
//
//output.open("encoded.rle2", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * real_data_size);
//output.close();
//
//delete[] buf_2;
//
//cout << "RLE2 end" << endl;

//********************************
//DECODING
//********************************
//int size2;
//		BWTdecoder decoder(block_size);
//
//		cout << "BWT start" << endl;
//
//		input.open("encoded.bwt", fstream::in|fstream::binary);
//		if(input.fail()){
//			cout << "Couldn't open input file: " << "encoded.bwt" << endl;
//			return EXIT_FAILURE;
//		} else{
//			input.seekg(0, fstream::end);
//			data_size = input.tellg();
//			if(data_size == -1){
//				cout << "Couldn't get input file size: " << "encoded.bwt" << endl;
//				return EXIT_FAILURE;
//			}
//			input.seekg(0, fstream::beg);
//		}
//
//		buf_1 = new uint8_t[data_size];
//		input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//		size2 = (reinterpret_cast<uint32_t*>(buf_2))[0];
//		buf_2 = new uint8_t[size2];
//		memset(buf_2, 0 , sizeof(uint8_t) * size2);
//
//		decoder.decode(reinterpret_cast<unsigned char*>(buf_1) + sizeof(uint32_t), buf_2, data_size);
//
//		output.open("decoded.bwt", fstream::out|fstream::trunc|fstream::binary);
//		output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//		output.close();
//
//		input.close();
//
//		delete[] buf_1;
//		delete[] buf_2;
//
//		cout << "BWT end" << endl;
//
//SecondStepAlgs::MTF mtf;
//
//cout << "MTF start" << endl;
//
//input.open("encoded.mtf", fstream::in);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.mtf" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.mtf" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//buf_2 = new uint8_t[data_size];
//memset(buf_2, 0 , sizeof(uint8_t) * data_size);
//
//mtf.decodeBuf(buf_1, buf_2, data_size);
//
//output.open("decoded.mtf", fstream::out|fstream::trunc);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * data_size);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "MTF end" << endl;
//
//SecondStepAlgs::HuffmanCoder hc;
//
//cout << "HC start" << endl;
//
//input.open("encoded.hc", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.hc" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.hc" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//size2 = (reinterpret_cast<int32_t*>(buf_1))[0];
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0 , sizeof(uint8_t) * size2);
//
//int32_t bits = (reinterpret_cast<int32_t*>(buf_1))[1];
//
//cout << "Bits = " << bits << endl;
//
//hc.decodeBuf(buf_1 + sizeof(uint32_t) * 2, buf_2, bits);
//
//output.open("decoded.hc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "HC end" << endl;
//
//SecondStepAlgs::DC dc;
//
//cout << "DC start" << endl;
//
//input.open("encoded.dc", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.dc" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.dc" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//uint32_t* buf_3 = reinterpret_cast<uint32_t*>(buf_1);
//
//size2 = buf_3[0];
//
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0 , sizeof(uint8_t) * size2);
//
//dc.decodeBuf(buf_3 + 1, buf_2, size2);
//
//output.open("decoded.dc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "DC end" << endl;
//
//SecondStepAlgs::IF invfreq;
//
//cout << "IF start" << endl;
//
//input.open("encoded.if", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.if" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.if" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//buf_3 = reinterpret_cast<uint32_t*>(buf_1);
//
//size2 = buf_3[0];
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0 , sizeof(uint8_t) * size2);
//
//invfreq.decodeBuf(buf_3 + 1, buf_2, size2);
//
//output.open("decoded.if", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "IF end" << endl;
//
//SecondStepAlgs::IFC ifc;
//
//cout << "IFC start" << endl;
//
//input.open("encoded.ifc", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.ifc" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.ifc" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//buf_2 = new uint8_t[data_size];
//memset(buf_2, 0 , sizeof(uint8_t) * data_size);
//
//ifc.decodeBuf(buf_1, buf_2, data_size);
//
//output.open("decoded.ifc", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * data_size);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "IFC end" << endl;
//
//SecondStepAlgs::RLE rle;
//
//cout << "RLE start" << endl;
//
//input.open("encoded.rle", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.rle" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.rle" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//size2 = (reinterpret_cast<uint32_t*>(buf_1))[0];
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0, sizeof(uint8_t) * size2);
//
//rle.decodeBuf(buf_1 + sizeof(uint32_t), buf_2, data_size - sizeof(uint32_t));
//
//output.open("decoded.rle", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "RLE end" << endl;
//
//SecondStepAlgs::RLE_2 rle2;
//
//cout << "RLE 2 start" << endl;
//
//input.open("encoded.rle2", fstream::in|fstream::binary);
//if(input.fail()){
//	cout << "Couldn't open input file: " << "encoded.rle2" << endl;
//	return EXIT_FAILURE;
//} else{
//	input.seekg(0, fstream::end);
//	data_size = input.tellg();
//	if(data_size == -1){
//		cout << "Couldn't get input file size: " << "encoded.rle2" << endl;
//		return EXIT_FAILURE;
//	}
//	input.seekg(0, fstream::beg);
//}
//
//buf_1 = new uint8_t[data_size];
//input.read(reinterpret_cast<char *>(buf_1), data_size);
//
//size2 = (reinterpret_cast<uint32_t*>(buf_1))[0];
//buf_2 = new uint8_t[size2];
//memset(buf_2, 0, sizeof(uint8_t) * size2);
//
//data_size = (reinterpret_cast<uint32_t*>(buf_1))[1];
//
//uint32_t * tmp = new uint32_t[(reinterpret_cast<uint32_t*>(buf_1))[2]];
//memcpy(tmp, buf_1 + 3 * sizeof(uint32_t) + data_size * sizeof(uint8_t), (reinterpret_cast<uint32_t*>(buf_1))[2] * sizeof(uint32_t));
//
//rle2.setRLE_buffer(tmp, (reinterpret_cast<uint32_t*>(buf_1))[2]);
//
//rle2.encodeBuf(buf_1  + sizeof(uint32_t) * 3 , buf_2, size2);
//
//output.open("decoded.rle2", fstream::out|fstream::trunc|fstream::binary);
//output.write(reinterpret_cast<char*>(buf_2), sizeof(uint8_t) * size2);
//output.close();
//
//input.close();
//
//delete[] buf_1;
//delete[] buf_2;
//
//cout << "RLE2 end" << endl;
