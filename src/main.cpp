/* 
 * File:   main.cpp
 * Author: Kacper Szkudlarek
 *
 * Created on 22 grudnia 2010, 13:51
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
#include "second_step/ArthmeticCoder.h"

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
	int32_t data_size = 0;
	uint8_t *buf_1;

	BWTcoder *encoder;
	BWTdecoder *decoder;

	SecondStepAlgs::DC* dc;
	SecondStepAlgs::RLE* rle;
	SecondStepAlgs::IF* invfreq;
	SecondStepAlgs::MTF* mtf;

	SecondStepAlgs::IFC* ifc;
	SecondStepAlgs::RLE_2* rle2;

	SecondStepAlgs::HuffmanCoder hc;
	SecondStepAlgs::coderData cd;

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
			ifc = new SecondStepAlgs::IFC();
			rle2 = new SecondStepAlgs::RLE_2();
			break;
		case '2':
//			cout << "RLE-0 + IFC + Huffman Coding" << endl;
			alg_type = 2;
			ifc = new SecondStepAlgs::IFC();
			rle = new SecondStepAlgs::RLE();
			break;
		case '3':
//			cout << "IFC + Huffman Coding" << endl;
			alg_type = 3;
			ifc = new SecondStepAlgs::IFC();
			break;
		case '4':
//			cout << "DC + RLE + Huffman Coding" << endl;
			alg_type = 4;
			dc = new SecondStepAlgs::DC();
			rle = new SecondStepAlgs::RLE();
			break;
		case '5':
//			cout << "DC + Huffman Coding" << endl;
			dc = new SecondStepAlgs::DC();
			alg_type = 5;
			break;
		case '6':
//			cout << "IF + RLE + Huffman Coding" << endl;
			invfreq = new SecondStepAlgs::IF();
			rle = new SecondStepAlgs::RLE();
			alg_type = 6;
			break;
		case '7':
//			cout << "IF + Huffman Coding" << endl;
			invfreq = new SecondStepAlgs::IF();
			alg_type = 7;
			break;
		case '8':
//			cout << "MTF + RLE + Huffman Coding" << endl;
			mtf = new SecondStepAlgs::MTF();
			rle = new SecondStepAlgs::RLE();
			alg_type = 8;
			break;
		case '9':
//			cout << "MTF + Huffman Coding" << endl;
			mtf = new SecondStepAlgs::MTF();
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

	int runs_size;
	uint8_t* tmp;
	uint32_t* runs;


	if(encoding){
		encoder = new BWTcoder(block_size);

		buf_1 = new uint8_t[data_size];

		input.read(reinterpret_cast<char *>(buf_1), data_size);

		int size2;
		if(data_size%block_size!=0)
			size2 = data_size+(static_cast<int>(data_size/block_size)+1)*sizeof(int);
		else
			size2 = data_size+static_cast<int>(data_size/block_size)*sizeof(int);

		size2+=sizeof(uint32_t);

		cd.in_buf = new uint8_t[size2];
		memset(cd.in_buf, 0 , sizeof(uint8_t) * size2);

		(reinterpret_cast<uint32_t*>(cd.in_buf))[0] = data_size;
		encoder->code(reinterpret_cast<unsigned char*>(buf_1), cd.in_buf + sizeof(uint32_t), data_size);

		cd.in_size = size2;

//		cd.in_buf = buf_1;
//		cd.in_size = data_size;

		switch(alg_type){
			case 1:
				// "RLE-2 + IFC + Huffman Coding"
				rle2->encodeBuf(&cd);

				runs_size = rle2->getSizeRLE_buffer();
				runs = rle2->getRLE_buffer();

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				ifc->encodeBuf(&cd);

				tmp = new uint8_t[cd.out_size + sizeof(uint32_t)*runs_size + 2*sizeof(uint32_t)];

				(reinterpret_cast<uint32_t*>(tmp))[0] = cd.out_size;
				(reinterpret_cast<uint32_t*>(tmp))[1] = runs_size;

				memcpy(tmp + sizeof(uint32_t)*2, runs, sizeof(uint32_t) * runs_size);
				memcpy(tmp + sizeof(uint32_t) * (runs_size + 2), cd.out_buf, cd.out_size);

				cd.out_buf = tmp;
				cd.out_size = (runs_size + 2) * sizeof(uint32_t)  + cd.out_size;

//				delete[] runs;

				break;
			case 2:
				// "RLE-0 + IFC + Huffman Coding"
				rle->encodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				ifc->encodeBuf(&cd);
				break;
			case 3:
				// "IFC + Huffman Coding"
				ifc->encodeBuf(&cd);
				break;
			case 4:
				// "DC + RLE + Huffman Coding";
				dc->encodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				rle->encodeBuf(&cd);
				break;
			case 5:
				// "DC + Huffman Coding"
				dc->encodeBuf(&cd);
				break;
			case 6:
				// "IF + RLE + Huffman Coding"
				invfreq->encodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				rle->encodeBuf(&cd);
				break;
			case 7:
				// "IF + Huffman Coding"
				invfreq->encodeBuf(&cd);
				break;
			case 8:
				// "MTF + RLE + Huffman Coding"
				mtf->encodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				rle->encodeBuf(&cd);
				break;
			case 9:
				// "MTF + Huffman Coding"
				mtf->encodeBuf(&cd);
				break;
		}

//		delete[] cd.in_buf;
		cd.in_buf = cd.out_buf;
		cd.in_size = cd.out_size;

		hc.encodeBuf(&cd);

		output.write((char*)cd.out_buf, cd.out_size);

//		delete[] cd.in_buf;
//		cd.in_buf = NULL;
//		delete[] cd.out_buf;
//		cd.out_buf = NULL;
//		delete[] buf_1;
	}

	if(decoding){
		decoder = new BWTdecoder(block_size);

		buf_1 = new uint8_t[data_size];
		input.read(reinterpret_cast<char *>(buf_1), data_size);

		cd.in_buf = buf_1;
		cd.in_size = data_size;

		hc.decodeBuf(&cd);

//		delete[] cd.in_buf;
		cd.in_buf = cd.out_buf;
		cd.in_size = cd.out_size;

		switch(alg_type){
			case 1:
				// "RLE-2 + IFC + Huffman Coding"
				cd.in_size = (reinterpret_cast<uint32_t*>(cd.in_buf))[0];
				runs_size = (reinterpret_cast<uint32_t*>(cd.in_buf))[1];

				tmp = new uint8_t[cd.in_size];
				runs = new uint32_t[runs_size];

				memcpy(runs, cd.in_buf + sizeof(uint32_t)*2, sizeof(uint32_t) * runs_size);
				memcpy(tmp, cd.in_buf + sizeof(uint32_t) * (runs_size + 2), cd.in_size);

				cd.in_buf = tmp;

				ifc->decodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				rle2->setRLE_buffer(runs, runs_size);
				rle2->decodeBuf(&cd);
				break;
			case 2:
				// "RLE-0 + IFC + Huffman Coding"
				ifc->decodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				rle->decodeBuf(&cd);
				break;
			case 3:
				// "IFC + Huffman Coding"
				ifc->decodeBuf(&cd);
				break;
			case 4:
				// "DC + RLE + Huffman Coding";
				rle->decodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				dc->decodeBuf(&cd);
				break;
			case 5:
				// "DC + Huffman Coding"
				dc->decodeBuf(&cd);
				break;
			case 6:
				// "IF + RLE + Huffman Coding"
				rle->decodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				invfreq->decodeBuf(&cd);
				break;
			case 7:
				// "IF + Huffman Coding"
				invfreq->decodeBuf(&cd);
				break;
			case 8:
				// "MTF + RLE + Huffman Coding"
				rle->decodeBuf(&cd);

//				delete[] cd.in_buf;
				cd.in_buf = cd.out_buf;
				cd.in_size = cd.out_size;

				mtf->decodeBuf(&cd);
				break;
			case 9:
				// "MTF + Huffman Coding"
				mtf->decodeBuf(&cd);
				break;
		}

		int size2 = (reinterpret_cast<uint32_t*>(cd.out_buf))[0];

		int codedSize;
		if(size2%block_size!=0)
			codedSize = size2+(static_cast<int>(size2/block_size)+1)*sizeof(int);
		else
			codedSize = size2+static_cast<int>(size2/block_size)*sizeof(int);

		uint8_t* buf_2 = new uint8_t[size2];
		memset(buf_2, 0 , sizeof(uint8_t) * size2);

		decoder->decode(reinterpret_cast<unsigned char*>(cd.out_buf + sizeof(uint32_t)), buf_2, codedSize);

		output.write((char*)buf_2, size2);

//		output.write((char*)cd.out_buf, cd.out_size);

//		delete[] cd.in_buf;
//		delete[] cd.out_buf;
//		delete[] buf_2;
	}

	return EXIT_SUCCESS;
}

