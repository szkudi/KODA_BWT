/* 
 * File:   main.cpp
 * Author: JM
 *
 * Created on 22 listopad 2010, 17:59
 */

#include <cstdlib>
#include <iostream>
#include <stack>
#include <utility>

using namespace std;

/**
 Struct for most significant digit radix sort impl.
 */
struct classForRadix {
    int dist;
    int start;
    int end;
    classForRadix(int d,int s,int e): dist(d),start(s),end(e) {}
    classForRadix(){}
};

/**
 * Does radix sort considering next bytes (first byte is the same for whole group)
 * @param char* input Array of bytes
 * @param int blockSize size of input
 * @param int* A Array of indexes from input (sorted input)
 * @param int start index from A - first element from the group to be sorted
 * @param int end index from A - one element after the last one to be sorted (sort el. from start to end-1)
 */
void radixSort(char* input,int blockSize,int* A,int start,int end)
{
    int* tmp = new int[end-start];
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
            if(dist==blockSize-1)//consider $(end of string, just to protect from looping)
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
            if(dist==blockSize-1)
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
                stackForRadix.push(classForRadix(dist+1,index[i]-bucket[i]+s,index[i]+s));
            }
        }
    }
    
}

/**
 * Sort suffixes using Itoh's algorithm
 * @param char* input Bytes to sort
 * @param int* A result - sorted lexicographically indexes of chars from input
 * @param int blockSize size of input
 */
void sortSuffixes(char* input, int* A,int blockSize)
{
    int bucketSizeX[256],bucketSizeY[256],bucketPtrX[256],bucketPtrY[256];

    for(int i=0;i<256;i++)//step 1-1
    {
        bucketSizeX[i]=0;
        bucketSizeY[i]=0;
    }

    blockSize--;
    for(int i=0;i<blockSize;i++)//step 1-2
    {
        if(input[i]>input[i+1])
        {
            bucketSizeX[input[i]]++;
        }
        else
        {
            bucketSizeY[input[i]]++;
        }
    }
    blockSize++;
    bucketSizeY[input[blockSize-1]]++;

    //step 1-3:
    bucketPtrX[0]=0;
    bucketPtrY[0]=0+bucketSizeX[0];
    for(int i=1;i<256;i++)
    {
        bucketPtrX[i]=bucketPtrY[i-1]+bucketSizeY[i-1];
        bucketPtrY[i]=bucketPtrX[i]+bucketSizeX[i];
    }

    //step1-4
    blockSize--;
    for(int i=0;i<blockSize;i++)
    {
        if(input[i]<=input[i+1])//in Y
        {
            A[bucketPtrY[input[i]]]=i;
            bucketPtrY[input[i]]++;
        }
    }
    blockSize++;

    A[bucketPtrY[input[blockSize-1]]]=blockSize-1;
    bucketPtrY[input[blockSize-1]]++;

    for(int i=0;i<256;i++)//step 2
    {
        if(bucketSizeY[i]>1)
        {
            //sort from bucketPtrY[i]-bucketSizeY+1 to bucketPtrY[i]
            radixSort(input,blockSize,A,(bucketPtrY[i]-bucketSizeY[i]),bucketPtrY[i]);
        }
    }

    for(int i=0;i<blockSize;i++)//step 3
    {
        //cout<<A[i]<<' '<<input[A[i]]<<endl;
        if(input[A[i]-1]>input[A[i]])//A[i]-1 in X
        {
            A[bucketPtrX[input[A[i]-1]]]=A[i]-1;
            bucketPtrX[input[A[i]-1]]++;
        }
    }

}

int BWTcode(char* input, char* output, int blockSize)
{
    int* A = new int[blockSize];
    int b1;
    cout<<input<<endl;

    sortSuffixes(input,A,blockSize);

    for(int i=0;i<blockSize;i++)
    {
        if(A[i]==0)
            output[i]=input[blockSize-1];
        else
            output[i]=input[A[i]-1];

        if(A[i]==1)
            b1 = i;
    }


    delete[] A;
    return b1;
}

void makeLookupInVP(char *input,int* lookup,int blockSize)
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

void BWTdecode(char* input, int b1, char* output, int blockSize)
{
    int lookup[256];
    int* VT = new int[blockSize];
    int idx;

    makeLookupInVP(input,lookup,blockSize);

    //cout<<"A:"<<endl;
    for(int i=0;i<blockSize;i++)
    {
        VT[ lookup[ input[i] ]++ ]=i;
    }

    idx=b1;
    for(int i=0;i<blockSize;i++)
    {
        output[i]=input[idx];
        idx=VT[idx];
    }

    delete[] VT;
}

/*
 * 
 */
int main(int argc, char** argv) {
    const int blockSize = 15;

    //char input[blockSize+1] = "ADBACCCABBBCCCA";
    char input[blockSize+1] = "BFFFBBBBFFFFFFB";
    //char input[blockSize+1] = "kajak";
    //char input[blockSize+1] = "ADAM";
    char* coded = new char[blockSize];

    int b1 = BWTcode(input,coded,blockSize);

    cout<<"B1:"<<b1<<endl;
    for(int i=0;i<blockSize;i++)
    {
        cout<<coded[i];
    }

    cout<<endl<<"--------"<<endl;

    char* decoded = new char[blockSize];
    BWTdecode(coded,b1,decoded,blockSize);
    for(int i=0;i<blockSize;i++)
    {
        cout<<decoded[i];
    }
    cout<<endl;

    //delete[] coded;
    delete[] decoded;
    return 0;
}


