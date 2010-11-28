/* 
 * File:   BWT.h
 * Author: JM
 *
 * Created on 28 listopad 2010, 18:50
 */

#ifndef BWT_H
#define	BWT_H

class BWTcoder {
public:
    BWTcoder(int blockSize);
    ~BWTcoder();
    /**
     * CAUTION: output needs dataSize*sizeof(char)+(ceil(dataSize/blockSize))*sizeof(int) bytes!
     * @param input
     * @param output
     * @param dataSize
     */
    void code(char* input, char* output, int dataSize);
private:
    int _BWTcodeBlock(char* input, char* output,int blockSize);

    /**
     * Sort suffixes using Itoh's algorithm
     * @param char* input Bytes to sort
     * @param int* A result - sorted lexicographically indexes of chars from input
     * @param int blockSize size of input
     */
    void _sortSuffixes(char* input, int* A,int blockSize);

    /**
     * Does radix sort considering next bytes (first byte is the same for whole group)
     * @param char* input Array of bytes
     * @param int blockSize size of input
     * @param int* A Array of indexes from input (sorted input)
     * @param int start index from A - first element from the group to be sorted
     * @param int end index from A - one element after the last one to be sorted (sort el. from start to end-1)
     */
    void _radixSort(char* input,int blockSize,int* A,int start,int end);

    int _blockSize;
    int* _A;

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
};

class BWTdecoder {
public:
    BWTdecoder(int blockSize);
    ~BWTdecoder();
    /**
     * @param input
     * @param output needs dataSize*sizeof(char)
     * @param dataSize
     */
    void decode(char* input, char* output, int dataSize);
private:
    void _BWTdecodeBlock(char* input, int b1, char* output, int blockSize);
    void _makeLookupInVP(char *input,int* lookup,int blockSize);

    int* _VT;
    int _blockSize;
};

#endif	/* BWT_H */

