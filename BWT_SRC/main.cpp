/* 
 * File:   main.cpp
 * Author: JM
 *
 * Created on 22 listopad 2010, 17:59
 */

#include <iostream>
#include <fstream>
#include <stack>
#include "BWT.h"
using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    int dataSize;
    //const int blockSize = 54;
    const int blockSize = 10600;

    char* input;

    //ifstream file (argv[1], ios::in|ios::binary|ios::ate);
    ifstream file (argv[1], ios::in|ios::binary|ios::ate);
    if (file.is_open())
    {
        dataSize = file.tellg();
        input = new char [dataSize];
        file.seekg (0, ios::beg);
        file.read (input, dataSize);
        file.close();
    }
    else
    {
        cout << "Unable to open file"<<endl;
        return 1;
    }

    int codedSize;
    if(dataSize%blockSize!=0)
        codedSize = dataSize+(static_cast<int>(dataSize/blockSize)+1)*sizeof(int);
    else
        codedSize = dataSize+static_cast<int>(dataSize/blockSize)*sizeof(int);

    unsigned char *output = new unsigned char[codedSize];
    unsigned char *out = new unsigned char[dataSize];

    BWTcoder coder(blockSize);
    BWTdecoder decoder(blockSize);

    coder.code(reinterpret_cast<unsigned char*>(input),output,dataSize);

    decoder.decode(output,out,codedSize);

    for(int i=0;i<dataSize;i++)
        cout<<out[i];

    /*fstream fileW ("/home/trapi/NetBeansProjects/KODA/main.copy", ios::out|ios::binary|ios::trunc);
    if (fileW.is_open())
    {
        fileW.seekg (0, ios::beg);
        fileW.write (out, dataSize);
        fileW.close();
    }
    else
    {
        cout << "Unable to open file"<<endl;
        return 1;
    }*/

    delete[] output;
    delete[] out;
    delete[] input;

    return 0;
}


