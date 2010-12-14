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
#include <list>
#include <algorithm>
#include <iostream>

using namespace std;

namespace SecondStepAlgs {

bool compareNodes(Node* n1, Node* n2){
	return !(n1->compareWithNode(n2));
}

HuffmanCoder::HuffmanCoder():
	tree(NULL){
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
		if(alphabet_weights[i] != -1){
			count ++;
			out_buf[out_pos++] = i;
			out_buf[out_pos++] = alphabet_weights[i];
			out_buf[out_pos++] = num_of_bits[i];
		}
	}
	out_buf[0] = count;
//	cout << count << " pos " << out_pos << endl;

	out_buf[out_pos] = 0;
	for(int i = 0; i < buf_size; ++i){
//		cout << i <<  " char = " << in_buf[i] << " bits: " << num_of_bits[in_buf[i]] << endl;
		if(num_of_bits[in_buf[i]] < bits_left){
//			cout << "if" << endl;
			out_buf[out_pos] |= (alphabet_weights[in_buf[i]] << (bits_left - num_of_bits[in_buf[i]]));
//			cout << (int)out_buf[out_pos] << endl;
			bits_left -= num_of_bits[in_buf[i]];
//			if(bits_left == 0){
//				bits_left = 8;
//				out_pos ++;
//			}
		}else{
//			cout << "else" << endl;
			tmp_bits = num_of_bits[in_buf[i]] - bits_left;
			out_buf[out_pos] |= (alphabet_weights[in_buf[i]] >> tmp_bits);
//			cout << (int)out_buf[out_pos] << endl;
			out_pos++;
			out_buf[out_pos] = 0;
			bits_left = 8;
			for(int j = 0; j <= (1.0*tmp_bits)/8; ++j){
				if(tmp_bits < bits_left){
					out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] << (bits_left - num_of_bits[in_buf[i]])) & 0xFF);
//					cout << (int)out_buf[out_pos] << " tmp = " << tmp_bits << endl;
					bits_left -= tmp_bits;
				}else{
					tmp_bits -= 8;
					out_buf[out_pos] |= ((alphabet_weights[in_buf[i]] >> tmp_bits) & 0xFF);
//					cout << (int)out_buf[out_pos] << endl;
					out_pos++;
					out_buf[out_pos] = 0;
				}
			}
		}
	}

	return (out_pos  - 3 * count) * 8 - bits_left;
}

void HuffmanCoder::decodeBuf(const uint8_t* in_buf, uint8_t* out_buf, int buf_size){

	uint8_t mask;
	int c = 0;
	int out_pos = 0;
	int tmp;
	int bits_num = 0;

	init();

	int count = in_buf[0];

	for(int i = 0; i < count; ++i){
		alphabet_weights[in_buf[3*i + 1]] = in_buf[3*i + 2];
		num_of_bits[in_buf[3*i + 1]] = in_buf[3*i + 3];
//		cout << in_buf[3*i + 1] << " wartość = " <<  alphabet_weights[in_buf[3*i + 1]] << " bitów = " << num_of_bits[in_buf[3*i + 1]] << endl;
	}

	count *= 3;
	count++; //Add first position containing number of alphabet elements

//	cout << count << endl;

	for(int i = 0; i < buf_size; ++i){
		mask = 1 << (7 - i%8);
		c = c << 1;
		bits_num++;
		c += (in_buf[count + i/8] & mask) > 0?1:0;
//		cout << "c = " << c << " bits = " << bits_num << " in buf = " << (int)in_buf[count + i/8] <<  endl;
		tmp = findCharacter(c, bits_num);
		if(tmp != -1){
//			cout << "char pos = " <<  (int)tmp << endl;
			out_buf[out_pos++] = tmp;
			c = 0;
			bits_num = 0;
		}
	}

}

int HuffmanCoder::findCharacter(int c, int bits_num){
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
	memset(alphabet_weights, -1, sizeof(int) * alphabet_size);
	memset(num_of_bits, 0, sizeof(int) * alphabet_size);

}

void HuffmanCoder::constructTree(){
	Node *n1, *n2, *n;
	list<Node *> nodes;

	for(int i = 0; i < alphabet_size; ++i){
		if(alphabet_weights[i] >= 0){
			nodes.push_back(new Node(i, alphabet_weights[i], true));
		}
	}


//	for(list<Node *>::iterator it = nodes.begin(); it != nodes.end(); ++it)
//		cout << (*it)->getWeight() << endl;

	while(nodes.size() > 1){
		nodes.sort(compareNodes);

		n1 = new Node(nodes.front());
		nodes.pop_front();
		n2 = new Node(nodes.front());
		nodes.pop_front();

		n = new Node(0, n1->getWeight() + n2->getWeight(), false, NULL, n1, n2);
		n1->setParent(n);
		n2->setParent(n);
		nodes.push_front(n);
	}

	tree2arrays(nodes.front(), 0, 0, false);

//	for(int i = 0; i < alphabet_size; ++i)
//		if(alphabet_weights[i] > -1)
//			cout << (char)i << " kod: " << alphabet_weights[i] << " ilość bitów: " << num_of_bits[i] << endl;


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

//	for(int i = 0; i < alphabet_size; ++i){
//		cout << i << " count = " << alphabet_weights[i] << endl;
//	}
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

