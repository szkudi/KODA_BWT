/* 
 * File:   main.cpp
 * Author: JM
 *
 * Created on 22 listopad 2010, 17:59
 */

#include <iostream>
#include "BWT.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    const int dataSize = 16;
    const int blockSize = 3;

    char input[dataSize] = "ADBACCCABBBCCCA";
    //char input[dataSize] = "BFFFBBBBFFFFFFB";
    //char input[blockSize+1] = "kajak";
    //char input[dataSize] = "ADAM";
    int codedSize;
    if(dataSize%blockSize!=0)
        codedSize = dataSize+(static_cast<int>(dataSize/blockSize)+1)*sizeof(int);
    else
        codedSize = dataSize+static_cast<int>(dataSize/blockSize)*sizeof(int);

    char *output = new char[codedSize];
    char *out = new char[dataSize];

    BWTcoder coder(blockSize);
    BWTdecoder decoder(blockSize);

    coder.code(input,output,dataSize);

    decoder.decode(output,out,codedSize);

    for(int i=0;i<dataSize;i++)
        cout<<out[i];

    delete[] output;
    delete[] out;

    return 0;
}


