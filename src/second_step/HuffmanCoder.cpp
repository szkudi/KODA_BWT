/*
 * HuffmanCoder.cpp
 *
 *  Created on: 13-12-2010
 *      Author: szkudi
 */

#include "HuffmanCoder.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

namespace SecondStepAlgs {

bool compareNodes(Node* n1, Node* n2){
	return !(n1->compareWithNode(n2));
}

HuffmanCoder::HuffmanCoder():
	tree(NULL), data_size(0){
}

HuffmanCoder::~HuffmanCoder() {
}

int HuffmanCoder::encodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size){

	int out_pos = 1;
	uint8_t bits_left = 8;
	int tmp_bits = 0;
	int count = 0;

	init();

	countWeights(in_buf, buf_size);

	constructTree();

	for(int i = 0; i < alphabet_size; ++i){
		if(num_of_bits[i] > 0){
			count ++;
			out_buf[out_pos++] = i;
			(reinterpret_cast<uint32_t*>(out_buf + out_pos))[0] = alphabet_weights[i];
			out_pos += sizeof(uint32_t);
			out_buf[out_pos++] = num_of_bits[i];
		}
	}
	out_buf[0] = count;

	out_buf[out_pos] = 0;
	for(int i = 0; i < buf_size; ++i){
		if(num_of_bits[in_buf[i]] < bits_left){
			out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] << (bits_left - num_of_bits[in_buf[i]])) & 0xFF);
			bits_left -= num_of_bits[in_buf[i]];
		}else{
			tmp_bits = num_of_bits[in_buf[i]] - bits_left;
			out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] >> tmp_bits) & 0xFF);
			out_pos++;
			out_buf[out_pos] = 0;
			bits_left = 8;
			for(int j = 0; j < ceil((1.0*tmp_bits)/8); ++j){
				if(tmp_bits < bits_left){
					out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] << (bits_left - tmp_bits)) & 0xFF);//num_of_bits[in_buf[i]])) & 0xFF);
					bits_left -= tmp_bits;
				}else{
					tmp_bits -= 8;
					out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] >> tmp_bits) & 0xFF);
					out_pos++;
					out_buf[out_pos] = 0;
				}
			}
		}
	}

	data_size = out_pos + 1;

	return (out_pos  - 2 * count - sizeof(uint32_t) * count) * 8 - bits_left;
}

int HuffmanCoder::decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size){

	uint8_t mask;
	uint32_t c = 0;
	int out_pos = 0;
	int in_pos = 0;
	int tmp;
	int bits_num = 0;

	init();

	int count = in_buf[in_pos++];

	for(int i = 0; i < count; ++i){
		tmp = in_buf[in_pos++];
		alphabet_weights[tmp] = (reinterpret_cast<const uint32_t*>(in_buf + in_pos))[0];
		in_pos += sizeof(uint32_t);
		num_of_bits[tmp] = in_buf[in_pos++];
	}

	for(int i = 0; i < buf_size; ++i){
		mask = 1 << (7 - i%8);
		c = c << 1;
		bits_num++;
		c += (in_buf[in_pos + i/8] & mask) != 0?1:0;
		tmp = findCharacter(c, bits_num);
		if(tmp != -1){
			out_buf[out_pos++] = (uint8_t)tmp;
			c = 0;
			bits_num = 0;
		}
	}

	return out_pos;

}

void HuffmanCoder::encodeBuf(coderData* data){

	int out8_pos = 0;
	int out16_pos = 0;
	uint8_t bits_in;
	int count = 0;
	int bits = 0;

	init();

	countWeights(data->in_buf, data->in_size);

	constructTree();

	data->out_buf = new uint8_t[data->in_size + 2 * used_alph_size + (3 + used_alph_size) * sizeof(uint32_t)];
	memset(data->out_buf, 0, data->in_size + 2 * used_alph_size + (3 + used_alph_size) * sizeof(uint32_t));

	uint8_t* out8 = data->out_buf + 3 * sizeof(uint32_t);

	for(int i = 0; i < alphabet_size; ++i){
		if(num_of_bits[i] > 0){
			count ++;
			out8[out8_pos++] = i;
			(reinterpret_cast<uint32_t*>(out8 + out8_pos))[0] = alphabet_weights[i];
			out8_pos += sizeof(uint32_t);
			out8[out8_pos++] = num_of_bits[i];
		}
	}
	cout << count << endl;

	uint16_t* out16 = reinterpret_cast<uint16_t*>(out8 + out8_pos);
	uint32_t* buff = new uint32_t;
	memset(buff, 0, sizeof(uint32_t));
	(*buff) = 0;

	bits_in = 0;
	for(unsigned int i = 0; i < data->in_size; ++i){
		bits += num_of_bits[data->in_buf[i]];
		(*buff) <<= num_of_bits[data->in_buf[i]];
		(*buff) += alphabet_weights[data->in_buf[i]];
		bits_in += num_of_bits[data->in_buf[i]];
		if(bits_in > 16){//Write out 16 bits
			out16[out16_pos++] = getBitsFromBuffer(*buff, bits_in);
			bits_in -= 16;
		}
	}

	(*buff) <<= (32 - bits_in);

	out16[out16_pos++] = (reinterpret_cast<uint16_t*>(buff))[1];
	out16[out16_pos++] = (reinterpret_cast<uint16_t*>(buff))[0];


	data->out_size = out8_pos + (out16_pos) * sizeof(uint16_t) + 3 * sizeof(uint32_t);

	data->var_1 = bits;

	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size; //Remember original input size
	(reinterpret_cast<uint32_t*>(data->out_buf))[1] = data->var_1; //Remember number of bits in output
	(reinterpret_cast<uint32_t*>(data->out_buf))[2] = count; //Remember number of alphabet elements
}

void HuffmanCoder::decodeBuf(coderData* data){

	uint32_t c = 0;
	int out_pos = 0;
	int in8_pos = 0;
	int in16_pos = 0;
	int tmp;
	int bits_num = 0;
	int bits_out = 0;

	init();

	uint8_t* in8 = data->in_buf + 3 * sizeof(uint32_t);

	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];
	data->var_1 = (reinterpret_cast<uint32_t*>(data->in_buf))[1];
	int count = (reinterpret_cast<uint32_t*>(data->in_buf))[2];

	data->out_buf = new uint8_t[data->out_size];

	for(int i = 0; i < count; ++i){
		tmp = in8[in8_pos++];
		alphabet_weights[tmp] = (reinterpret_cast<const uint32_t*>(in8 + in8_pos))[0];
		in8_pos += sizeof(uint32_t);
		num_of_bits[tmp] = in8[in8_pos++];
	}

	uint32_t* buff = new uint32_t;
	memset(buff, 0, sizeof(uint32_t));

	uint16_t* in16 = reinterpret_cast<uint16_t*>(in8 + in8_pos);

	(reinterpret_cast<uint16_t*>(buff))[1] = in16[in16_pos++];
	(reinterpret_cast<uint16_t*>(buff))[0] = in16[in16_pos++];

	cout << data->var_1 << endl;

	for(unsigned int i = 0; i < data->var_1; ++i){
		c <<= 1;
		bits_num++;
		if((*buff) & 0x80000000){
			c++;
		}
		tmp = findCharacter(c, bits_num);
		if(tmp != -1){
			data->out_buf[out_pos++] = tmp;
			c = 0;
			bits_num = 0;
		}
		(*buff) <<= 1;
		bits_out++;
		if(bits_out >= 16){
			(reinterpret_cast<uint16_t*>(buff))[0] = in16[in16_pos++];
			bits_out -= 16;
		}

	}

}

uint32_t HuffmanCoder::generateMask(int size){
	uint32_t ret = 0;
	for(int i = 0; i < size; ++i){
		ret <<= 1;
		ret++;
	}
	return ret;
}

uint16_t HuffmanCoder::getBitsFromBuffer(uint32_t buff, int bits){
	buff >>= (bits - 16);
	buff &= 0xFFFF;
	return (uint16_t)buff;

}


int HuffmanCoder::getData_size(){
	return data_size;
}

int HuffmanCoder::findCharacter(uint32_t c, int bits_num){
	int i;
	bool found = false;

	for(i = 0; i < alphabet_size; ++i)
		if(alphabet_weights[i] == c && num_of_bits[i] == bits_num){
			found = true;
			break;
		}

	if(found)
		return i;
	else
		return -1;
}

void HuffmanCoder::setAlphabet(int *alphabet){
	memcpy(alphabet_weights, alphabet, sizeof(int) * alphabet_size);
}

int* HuffmanCoder::getAlphabet(){
	int *ret = new int[alphabet_size];
	for(int i = 0; i <  alphabet_size; ++i){
		ret[i] = alphabet_weights[i];
	}

	return ret;
}

void HuffmanCoder::init(){
	memset(alphabet_weights, 0, sizeof(int) * alphabet_size);
	memset(num_of_bits, -1, sizeof(int) * alphabet_size);

}

void HuffmanCoder::constructTree(){
	Node *n1, *n2, *n;
	list<Node *> nodes;
	used_alph_size = 0;

	for(int i = 0; i < alphabet_size; ++i){
		if(alphabet_weights[i] > 0){
			nodes.push_back(new Node(i, alphabet_weights[i], true));
			used_alph_size++;
		}
	}

	while(nodes.size() > 1){
		nodes.sort(compareNodes);

		n1 = new Node(nodes.front());
		nodes.pop_front();
		n2 = new Node(nodes.front());
		nodes.pop_front();

		n = new Node(0, n1->getWeight() + n2->getWeight(), false, NULL, n1, n2);
		n1->setParent(n);
		n2->setParent(n);
		nodes.push_back(n);
	}

	tree2arrays(nodes.front(), 0, 0, false);
}

void HuffmanCoder::tree2arrays(Node *tree, int level, int num, bool right){
	num = num << 1;
	if(right)
		num += 1;
	if(tree->getLeft() != NULL)
		tree2arrays(tree->getLeft(), level + 1, num, false);
	if(tree->getRight() != NULL)
		tree2arrays(tree->getRight(), level + 1, num, true);

	if(tree->getUseValue()){
		alphabet_weights[tree->getC()] = num;
		num_of_bits[tree->getC()] = level;
	}
}

void HuffmanCoder::countWeights(const uint8_t* in_buf, int buf_size){
	for(int i = 0; i < buf_size; ++i){
		alphabet_weights[in_buf[i]]++;
	}
}

/********************************************************/
/*******************NODE CLASS***************************/
/********************************************************/

Node::Node():
		left(NULL), right(NULL), parent(NULL){
}

Node::Node(Node * node):
	c(node->getC()), weight(node->getWeight()), use_value(node->getUseValue()), left(node->getLeft()), right(node->getRight()), parent(node->getParent()){

}

Node::Node(uint8_t c, int weight, bool use_value, Node *parent, Node *left, Node *right):
		c(c), weight(weight), use_value(use_value), left(left), right(right), parent(parent){
}

Node::~Node(){
}

bool Node::compareWithNode(Node *node){
	return this->weight > node->getWeight();
}

uint8_t Node::getC() const{
	return c;
}

Node *Node::getLeft() const{
	return left;
}

Node *Node::getParent() const{
	return parent;
}

Node *Node::getRight() const{
	return right;
}

int Node::getWeight() const{
	return weight;
}

bool Node::getUseValue() const{
	return this->use_value;
}

void Node::setC(uint8_t c){
	this->c = c;
}

void Node::setLeft(Node *left){
	this->left = left;
}

void Node::setParent(Node *parent){
	this->parent = parent;
}

void Node::setRight(Node *right){
	this->right = right;
}

void Node::setWeight(int weight){
	this->weight = weight;
}

void Node::setUseValue(bool val){
	this->use_value = val;
}

}

