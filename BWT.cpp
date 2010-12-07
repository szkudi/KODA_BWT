/* 
 * File:   BWT.cpp
 * Author: JM
 * 
 * Created on 28 listopad 2010, 18:50
 */

#include "BWT.h"
#include <stack>
#include <iostream>
#include <vector>

using namespace std;

BWTcoder::BWTcoder(int blockSize): _blockSize(blockSize) {
    _A = new int[_blockSize];
}

BWTcoder::~BWTcoder() {
    delete[] _A;
}

void BWTcoder::code(unsigned char* input, unsigned char* output, int dataSize)
{
    int b1;
    int *temp;
    int i,j;
    for(i=dataSize,j=0;i-_blockSize>0;i-=_blockSize,j++)
    {
        b1 = _BWTcodeBlock(&input[j*_blockSize],&output[j*(_blockSize+sizeof(int))],_blockSize);
        
        //store b1:
        temp = reinterpret_cast<int*>(&output[(j+1)*_blockSize+j*sizeof(int)]);
        *temp = b1;
    }
    //store last block:
    b1 = _BWTcodeBlock(&input[j*_blockSize],&output[j*(_blockSize+sizeof(int))],i);
    //cout<<j*_blockSize<<' '<<j*(_blockSize+sizeof(int))<<' '<<j*_blockSize+j*sizeof(int)+i<<endl;
    temp = reinterpret_cast<int*>(&output[j*_blockSize+j*sizeof(int)+i]);
    *temp = b1;
}

int BWTcoder::_BWTcodeBlock(unsigned char* input, unsigned char* output,int blockSize)
{
    if(blockSize==1)
    {
        output[0]=input[0];
        return -1;
    }

    int b1;
    //cout<<input<<endl;

    _sortSuffixes(input,_A,blockSize);

    for(int i=0;i<blockSize;i++)
    {
        if(_A[i]==0)
            output[i]=input[blockSize-1];
        else
            output[i]=input[_A[i]-1];

        if(_A[i]==1)
            b1 = i;
    }

    return b1;
}

/**
 * Does radix sort considering next bytes (first byte is the same for whole group)
 * @param char* input Array of bytes
 * @param int blockSize size of input
 * @param int* A Array of indexes from input (sorted input)
 * @param int start index from A - first element from the group to be sorted
 * @param int end index from A - one element after the last one to be sorted (sort el. from start to end-1)
 */
void BWTcoder::_radixSort(unsigned char* input,int blockSize,int* A,int start,int end)
{
    int* tmp = new int[end-start+1];
    stack<classForRadix> stackForRadix;
    stackForRadix.push(classForRadix(1,start,end));

    int dist;
    int s,e;
    int bucket[257];
    int index[257];
    classForRadix c;

    while(!stackForRadix.empty())
    {
        c=stackForRadix.top();
        stackForRadix.pop();

        dist=c.dist;
        s=c.start;
        e=c.end;

        for(int i=0;i<257;i++)
        {
            bucket[i]=0;
        }

        for(int i=s;i<e;i++)//count elements
        {
            if(dist==blockSize)//consider $(end of string, just to protect from looping)
            {
                bucket[256]++;
            }
            else
            {
                bucket[input[(A[i]+dist)%blockSize]]++;
            }
        }

        index[256]=0;
        index[0]=bucket[256];
        for(int i=1;i<256;i++)
        {
          index[i]=index[i-1]+bucket[i-1];
        }

        for(int i=s;i<e;i++)
        {
            if(dist==blockSize)
            {
                tmp[index[256]]=A[i];
                index[256]++;
            }
            else
            {
                tmp[index[input[(A[i]+dist)%blockSize]]]=A[i];
                index[input[(A[i]+dist)%blockSize]]++;
            }
        }

        for(int i=s,j=0;i<e;i++,j++)
            A[i]=tmp[j];

        for(int i=256;i>=0;i--)//this direction, because it's a stack
        {
            if(bucket[i]>1)
            {
                //cout<<"I must sort "<<static_cast<char>(i)<<' '<<index[i]-bucket[i]+s<<' '<<index[i]+s<<endl;
                if(dist+1<blockSize)
                    stackForRadix.push(classForRadix(dist+1,index[i]-bucket[i]+s,index[i]+s));
            }
        }
    }

    delete[] tmp;

}

/**
 * Sort suffixes using Itoh's algorithm
 * @param char* input Bytes to sort
 * @param int* A result - sorted lexicographically indexes of chars from input
 * @param int blockSize size of input
 */
void BWTcoder::_sortSuffixes(unsigned char* input, int* A,int blockSize)
{
    int bucketSizeX[256],bucketSizeY[256],bucketPtrX[256],bucketPtrY[256];

    for(int i=0;i<blockSize;i++)
        A[i]=-1;

    for(int i=0;i<256;i++)//step 1-1
    {
        bucketSizeX[i]=0;
        bucketSizeY[i]=0;
    }

    //blockSize--;
    for(int i=0;i<blockSize;i++)//step 1-2
    {///e:eae
        if(input[i]>input[(i+1)%blockSize])
        {
            bucketSizeX[input[i]]++;
        }
        else
        {
            bucketSizeY[input[i]]++;
        }
    }
    /*blockSize++;
    bucketSizeY[input[blockSize-1]]++;*/

    //step 1-3:
    bucketPtrX[0]=0;
    bucketPtrY[0]=0+bucketSizeX[0];
    for(int i=1;i<256;i++)
    {
        bucketPtrX[i]=bucketPtrY[i-1]+bucketSizeY[i-1];
        bucketPtrY[i]=bucketPtrX[i]+bucketSizeX[i];
    }

    //step1-4
    //blockSize--;
    for(int i=0,j=0;i<blockSize;i++)
    {
        j=i+1;
        if(i+1==blockSize)
            j=0;

        if(input[i]<=input[j])//in Y
        {
            A[bucketPtrY[input[i]]]=i;
            bucketPtrY[input[i]]++;
        }
    }
    /*blockSize++;

    A[bucketPtrY[input[blockSize-1]]]=blockSize-1;
    bucketPtrY[input[blockSize-1]]++;*/

    for(int i=0;i<256;i++)//step 2
    {
        if(bucketSizeY[i]>1)
        {
            //sort from bucketPtrY[i]-bucketSizeY[i]+1 to bucketPtrY[i]
            //cout<<"sort from "<<bucketPtrY[i]-bucketSizeY[i]+1<<" to "<<bucketPtrY[i]<<endl;
            _radixSort(input,blockSize,A,(bucketPtrY[i]-bucketSizeY[i]),bucketPtrY[i]);
        }
    }

    for(int i=0,j=0;i<blockSize;i++)//step 3
    {
        if(A[i]>0)
        {
            //cout<<A[i]<<' '<<input[A[i]]<<endl;
            if(input[A[i]-1]>input[A[i]])//A[i]-1 in X
            {
                A[bucketPtrX[input[A[i]-1]]]=A[i]-1;
                bucketPtrX[input[A[i]-1]]++;
            }
        }
        else//A[i]==0
        {
            if(input[blockSize-1]>input[A[i]])//A[i]-1 in X
            {
                A[bucketPtrX[input[blockSize-1]]]=blockSize-1;
                bucketPtrX[input[blockSize-1]]++;
            }
        }
    }

}

BWTdecoder::BWTdecoder(int blockSize): _blockSize(blockSize) {
    _VT = new int[_blockSize];
}

BWTdecoder::~BWTdecoder() {
    delete[] _VT;
}

void BWTdecoder::decode(unsigned char* input, unsigned char* output, int dataSize)
{
    int b1;
    int *temp;
    int i,j;
    int intSize=sizeof(int);
    for(i=dataSize,j=0;(i-_blockSize-intSize)>0;i-=(_blockSize+sizeof(int)),j++)
    {
        temp = reinterpret_cast<int*>(&input[(j+1)*_blockSize+j*sizeof(int)]);
        b1 = *temp;
        _BWTdecodeBlock(&input[j*(_blockSize+sizeof(int))],b1,&output[j*_blockSize],_blockSize);
    }
    temp = reinterpret_cast<int*>(&input[j*(_blockSize+sizeof(int))+i-sizeof(int)]);
    b1 = *temp;
    _BWTdecodeBlock(&input[j*(_blockSize+sizeof(int))],b1,&output[j*_blockSize],i-intSize);
    //cout<<j*_blockSize<<' '<<j*(_blockSize+sizeof(int))+i-sizeof(int)<<' '<<j*(_blockSize+sizeof(int))+i-sizeof(int)<<endl;
}

void BWTdecoder::_BWTdecodeBlock(unsigned char* input, int b1, unsigned char* output, int blockSize)
{
    if(blockSize==1)
    {
        output[0]=input[0];
        return;
    }

    int lookup[256];
    int idx;

    _makeLookupInVP(input,lookup,blockSize);

    //cout<<"A:"<<endl;
    for(int i=0;i<blockSize;i++)
    {
        _VT[ lookup[ input[i] ]++ ]=i;
    }

    idx=b1;
    for(int i=0;i<blockSize;i++)
    {
        output[i]=input[idx];
        idx=_VT[idx];
    }

}

void BWTdecoder::_makeLookupInVP(unsigned char *input,int* lookup,int blockSize)
{
    int bucket[256];

    for(int i=0;i<256;i++)
    {
        bucket[i]=0;
    }

    for(int i=0;i<blockSize;i++)//count elements
    {
        bucket[input[i]]++;
    }

    lookup[0]=0;
    for(int i=1;i<256;i++)
    {
        lookup[i]=lookup[i-1]+bucket[i-1];
    }
}