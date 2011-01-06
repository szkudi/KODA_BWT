/*
 * HuffmanCoder.h
 *
 *  Created on: 13-12-2010
 *      Author: szkudi
 */

#ifndef HUFFMANCODER_H_
#define HUFFMANCODER_H_

#include <stdint.h>
#include <cstdlib>
#include "SecondStepAlg.h"

namespace SecondStepAlgs {

class Node;

class HuffmanCoder {
public:
	/*!
	 * Class constructor
	 */
	HuffmanCoder();
	/*!
	 * Class destructor
	 */
	virtual ~HuffmanCoder();
	/*!
	 * Method used to encode given data
	 *
	 * \param in_buf - pointer to data, which should be encode
	 * \param out_buf - pointer where encoded data will be returned
	 * \param buf_size - size of data
	 *
	 * \return Return number of bits in output. Encoded data is returned via pointer given to method
	 */
	int encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void encodeBuf(coderData* data);
	/*!
	 * Method used to decode given encoded data
	 *
	 * \param in_buf - pointer to data, which should be decode
	 * \param out_buf - pointer where decoded data will be returned
	 * \param buf_size - size of data (in bits)
	 *
	 * \return Decoded data is returned via pointer given to method
	 */
	int decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size);
	void decodeBuf(coderData* data);


	int* getAlphabet();
	void setAlphabet(int* alphabet);
	int getData_size();

private:
	/*!
	 * Constructs tree to alphabet_weights non zero elements.
	 */
	void constructTree();

	/*!
	 * Count weight of each alphabet_weights symbol.
	 *
	 * \param in_buf input data buffer
	 * \param buf_size input buffer size
	 */
	void countWeights(const uint8_t* in_buf, int buf_size);

	/*!
	 * Recursive method to fill arrays with data generated from binary tree
	 *
	 * \param tree - root node of analyzed tree
	 * \param level - how deep in tree we are
	 * \param num - already counted huffman code for element
	 * \param right - flag if connection add 0 (false) or 1 (true) to num
	 */
	void tree2arrays(Node *tree, int level, int num, bool right);

	int findCharacter(uint32_t c, int bits_num);

	uint32_t generateMask(int size);

	uint16_t getBitsFromBuffer(uint32_t buff, int bits);

	void init();

	Node* tree;

	static const int alphabet_size = 256;

	///Array containing used alphabet_weights
	uint32_t alphabet_weights[alphabet_size];
	int32_t num_of_bits[alphabet_size];

	int data_size;
	int used_alph_size;
};

class Node {
public:
	/*!
	 * Class constructor
	 */
	Node();
	Node(Node* node);
	Node(uint8_t c, int weight, bool use_value = false, Node *parent = NULL, Node* left = NULL, Node* right = NULL);
	/*!
	 * Class destructor
	 */
	~Node();

	/*!
	 * Method to compare nodes weights
	 *
	 * \return true if this node has greater weight and false otherwise
	 */
	bool compareWithNode(Node* node);
    uint8_t getC() const;
    Node *getLeft() const;
    Node *getParent() const;
    Node *getRight() const;
    int getWeight() const;
    bool getUseValue() const;
    void setC(uint8_t c);
    void setLeft(Node *left);
    void setParent(Node *parent);
    void setRight(Node *right);
    void setWeight(int weight);
    void setUseValue(bool val);
private:
	uint8_t c;
	int weight;
    bool use_value;
	Node* left;
	Node* right;
	Node* parent;
};

}//namespace

#endif /* HUFFMANCODER_H_ */

