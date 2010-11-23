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

void radixSort(char* input,int blockSize,int* A,int start,int end)
{
    cout<<"Start"<<start<<' '<<end<<endl;
    for(int i=start;i<end;i++)
        cout<<input[A[i]]<<' '<<input[A[i]+1]<<A[i]<<"\t";
    cout<<endl;

    int* tmp = new int[end-start];

    int dist=1;
    int up=start;
    int bucket[257];
    for(int i=0;i<257;i++)
    {
        bucket[i]=0;
    }

    for(int i=start;i<end;i++)
    {
        if(A[i]+dist==blockSize)
        {
            bucket[256]++;
        }
        else
        {
            bucket[input[A[i]+dist]]++;
        }
    }

    int index[257];
    index[0]=bucket[256];

    for(int i=1;i<256;i++)
    {
      index[i]=index[i-1]+bucket[i-1];
    }

    //cout<<"B:"<<index['B']<<"C:"<<index['C']<<"D:"<<index['D']<<endl;

    for(int i=start;i<end;i++)
    {
        if(A[i]+dist==blockSize)
        {
            tmp[index[256]]=A[i];
            index[256]++;
        }
        else
        {
            //cout<<static_cast<char>(A[i]+dist)<<index[input[A[i]+dist]]<<'|';
            tmp[index[input[A[i]+dist]]]=A[i];
            index[input[A[i]+dist]]++;
        }
    }

    for(int i=0;i<end-start;i++)
    {
        cout<<tmp[i]<<' ';
    }
    cout<<endl;

    for(int i=start,j=0;i<end;i++,j++)
        A[i]=tmp[j];

    for(int i=0;i<257;i++)
    {
        if(bucket[i]>1)
        {
            cout<<"Musze sortowac "<<static_cast<char>(i)<<' '<<index[i]-bucket[i]+start<<' '<<index[i]+start<<endl;
        }
        //if(bucket[i]!=0)
          //  cout<<static_cast<char>(i)<<bucket[i]<<endl;
    }
}

void sortSuffixes(char* input, int blockSize)
{
    int* A = new int[blockSize];
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

    cout<<bucketPtrX['A']<<' '<<bucketPtrX['B']<<' '<<bucketPtrX['C']<<' '<<bucketPtrX['D']<<endl;
    cout<<bucketSizeX['A']<<' '<<bucketSizeX['B']<<' '<<bucketSizeX['C']<<' '<<bucketSizeX['D']<<endl;

    cout<<bucketPtrY['A']<<' '<<bucketPtrY['B']<<' '<<bucketPtrY['C']<<' '<<bucketPtrY['D']<<endl;
    cout<<bucketSizeY['A']<<' '<<bucketSizeY['B']<<' '<<bucketSizeY['C']<<' '<<bucketSizeY['D']<<endl;

    for(int i=0;i<blockSize;i++)
    {
        cout<<A[i]<<' '<<input[A[i]]<<endl;
    }

    for(int i=0;i<256;i++)
    {
        if(bucketSizeY[i]>2)
        {
            //sort from bucketPtrY[i]-bucketSizeY+1 to bucketPtrY[i]
            radixSort(input,blockSize,A,(bucketPtrY[i]-bucketSizeY[i]),bucketPtrY[i]);
        }
    }

}

/*
 * 
 */
int main(int argc, char** argv) {
    const int blockSize = 15;

    char input[blockSize+1] = "ADBACCCABBBCCCA";

    cout<<input<<endl;

    sortSuffixes(input,blockSize);

    return 0;
}


