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
//	cout << "Count = " << count << endl;

	for(int i = 0; i < count; ++i){
		tmp = in_buf[in_pos++];
		alphabet_weights[tmp] = (reinterpret_cast<const uint32_t*>(in_buf + in_pos))[0];
		in_pos += sizeof(uint32_t);
		num_of_bits[tmp] = in_buf[in_pos++];
//		cout <<(char)tmp << " wartosc = " <<  alphabet_weights[tmp] << " bitow " << num_of_bits[tmp] << endl;
	}

//	count *= 3;
//	count++; //Add first position containing number of alphabet elements

//	cout << count << endl;

	for(int i = 0; i < buf_size; ++i){
		mask = 1 << (7 - i%8);
		c = c << 1;
		bits_num++;
		c += (in_buf[in_pos + i/8] & mask) != 0?1:0;
//		cout << "Mask = " << (int)mask << " wartosc = " << ((in_buf[in_pos + i/8] & mask) != 0?1:0) << " c = " << c << endl;
//		cout << "c = " << c << " bits = " << bits_num << " in buf = " << (int)in_buf[count + i/8] <<  endl;
		tmp = findCharacter(c, bits_num);
		if(tmp != -1){
//			cout << "Znak nr = " << out_pos << " bitow = " << bits_num << endl;
//			cout << "char pos = " <<  (int)tmp << " " << (char)tmp << endl;
			out_buf[out_pos++] = (uint8_t)tmp;
			c = 0;
			bits_num = 0;
		}
	}

	return out_pos;

}

void HuffmanCoder::encodeBuf(coderData* data){

	int out_pos = 1;
	uint8_t bits_left = 8;
	int tmp_bits = 0;
	int count = 0;
	int bits = 0;

	init();

	countWeights(data->in_buf, data->in_size);

	constructTree();

	data->out_buf = new uint8_t[data->in_size + 2 * used_alph_size + (2 + used_alph_size) * sizeof(uint32_t)];
	memset(data->out_buf, 0, data->in_size + 2 * used_alph_size + (2 + used_alph_size) * sizeof(uint32_t));

	uint8_t* out = data->out_buf + 2 * sizeof(uint32_t);

	for(int i = 0; i < alphabet_size; ++i){
		if(num_of_bits[i] > 0){
			count ++;
			out[out_pos++] = i;
			(reinterpret_cast<uint32_t*>(out + out_pos))[0] = alphabet_weights[i];
			out_pos += sizeof(uint32_t);
			out[out_pos++] = num_of_bits[i];
//			cout << (char)i << " wartosc = " << alphabet_weights[i] << " bitow = " << num_of_bits[i] << endl;;
		}
	}
	out[0] = count;

	uint32_t* out32 = reinterpret_cast<uint32_t*>(out + out_pos);

	bits_left = 32;
	int out32_pos = 0;
	for(unsigned int i = 0; i < data->in_size; ++i){
		bits += num_of_bits[data->in_buf[i]];
		if(num_of_bits[data->in_buf[i]] < bits_left){
			out32[out32_pos] |= alphabet_weights[data->in_buf[i]] << (bits_left - num_of_bits[data->in_buf[i]]);
//			cout << i << " Less" << " bits " << num_of_bits[data->in_buf[i]]  << " char " <<  alphabet_weights[data->in_buf[i]] <<  " out " << out32[out32_pos] <<  endl;
			bits_left -= num_of_bits[data->in_buf[i]];
		}else{
			tmp_bits = num_of_bits[data->in_buf[i]] - bits_left;
			out32[out32_pos] |= alphabet_weights[data->in_buf[i]] >> tmp_bits;

//			cout << i << " Else" << " bits " << num_of_bits[data->in_buf[i]]  << " tmp_bits " << tmp_bits << " char " <<  alphabet_weights[data->in_buf[i]] << " out " << out32[out32_pos] << endl;

			out32_pos++;
			bits_left = 32;

			if(tmp_bits){
				out32[out32_pos] |= alphabet_weights[data->in_buf[i]] << (bits_left - tmp_bits);
				bits_left -= tmp_bits;
			}
		}
	}

	cout << bits << endl;

//	out[out_pos] = 0;
//	for(unsigned int i = 0; i < data->in_size; ++i){
//		bits += num_of_bits[data->in_buf[i]];
//		if(num_of_bits[data->in_buf[i]] < bits_left){
//			out[out_pos] += (uint32_t)(alphabet_weights[data->in_buf[i]] * pow(2, bits_left - num_of_bits[data->in_buf[i]])) % 256;
//			bits_left -= num_of_bits[data->in_buf[i]];
//		}else{
//			tmp_bits = num_of_bits[data->in_buf[i]] - bits_left;
//			out[out_pos] +=  (uint32_t)(alphabet_weights[data->in_buf[i]] / pow(2, tmp_bits)) % 256;
//			out_pos++;
////			out[out_pos] = 0;
//			bits_left = 8;
//			for(int j = 0; j < ceil(1.0*tmp_bits/8); ++j){
//				if(tmp_bits > bits_left){
//					tmp_bits -= bits_left;
//					out[out_pos] +=  (uint32_t)(alphabet_weights[data->in_buf[i]] / pow(2, tmp_bits)) % 256;
//					out_pos++;
////					out[out_pos] = 0;
//				}else{
//					out[out_pos] +=  (uint32_t)(alphabet_weights[data->in_buf[i]] * pow(2, bits_left - tmp_bits)) % 256;
//					bits_left -= tmp_bits;
//				}
//			}
//		}
//	}

//	if(bits_left != 8)
//		data->out_size = out_pos + 1 + 2 * sizeof(uint32_t);
//	else
//		data->out_size = out_pos + 2 * sizeof(uint32_t);

	data->out_size = out_pos + (out32_pos + 3) * sizeof(uint32_t);

	data->var_1 = bits;//(out_pos - 2 * count - sizeof(uint32_t) * count) * 8 - bits_left;
//	cout << bits << " " << data->var_1 << endl;

	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size; //Remember original input size
	(reinterpret_cast<uint32_t*>(data->out_buf))[1] = data->var_1; //Remember number of bits in output




//	for(unsigned int i = 0; i < data->in_size; ++i){
////		cout << "Znak nr = " << i << " bitów = " << num_of_bits[data->in_buf[i]] << endl;
////		if(i == 105 + sizeof(uint32_t))
////			cout <<  "bitów = " << num_of_bits[data->in_buf[i]] << " znak = " <<  (int)data->in_buf[i] << " bits_left = " << (int)bits_left << endl;
//		if(num_of_bits[data->in_buf[i]] < bits_left){
//			out[out_pos] |= ((alphabet_weights[data->in_buf[i]] << (bits_left - num_of_bits[data->in_buf[i]])) & 0xFF);
//			bits_left -= num_of_bits[data->in_buf[i]];
//		}else{
//			tmp_bits = num_of_bits[data->in_buf[i]] - bits_left;
//			out[out_pos] |= ((alphabet_weights[data->in_buf[i]] >> tmp_bits) & 0xFF);
//			out_pos++;
////			out[out_pos] = 0;
//			bits_left = 8;
//			for(int j = 0; j < ceil((1.0*tmp_bits)/8); ++j){
//				if(tmp_bits < bits_left){
//					out[out_pos] |= ((alphabet_weights[data->in_buf[i]] << (bits_left - tmp_bits)) & 0xFF);//num_of_bits[in_buf[i]])) & 0xFF);
//					bits_left -= tmp_bits;
//				}else{
//					tmp_bits -= 8;
//					out[out_pos] |= ((alphabet_weights[data->in_buf[i]] >> tmp_bits) & 0xFF);
//					out_pos++;
////					out[out_pos] = 0;
//				}
//			}
//		}
////		if(i == 105 + sizeof(uint32_t))
////			cout << "wpisany znak = " << alphabet_weights[data->in_buf[i]] << " bits_left = " << (int)bits_left << " " << (int)out[out_pos] << " bajt " << out_pos  << endl;
//	}
//
//	if(bits_left != 8)
//		data->out_size = out_pos + 1 + 2 * sizeof(uint32_t);
//	else
//		data->out_size = out_pos + 2 * sizeof(uint32_t);
//
//
//	data->var_1 = (out_pos  - 2 * count - sizeof(uint32_t) * count) * 8 - bits_left;
//
//	(reinterpret_cast<uint32_t*>(data->out_buf))[0] = data->in_size; //Remember original input size
//	(reinterpret_cast<uint32_t*>(data->out_buf))[1] = data->var_1; //Remember number of bits in output
}

void HuffmanCoder::decodeBuf(coderData* data){

//	uint8_t mask;
	uint32_t mask;
	uint32_t c = 0;
	int out_pos = 0;
	int in_pos = 0;
	int tmp;
	int bits_num = 0;

	init();

	uint8_t* in = data->in_buf + 2 * sizeof(uint32_t);

	data->out_size = (reinterpret_cast<uint32_t*>(data->in_buf))[0];
	data->var_1 = (reinterpret_cast<uint32_t*>(data->in_buf))[1];

//	cout << data->var_1 << endl;

	data->out_buf = new uint8_t[data->out_size];

	int count = in[in_pos++];

	for(int i = 0; i < count; ++i){
		tmp = in[in_pos++];
		alphabet_weights[tmp] = (reinterpret_cast<const uint32_t*>(in + in_pos))[0];
		in_pos += sizeof(uint32_t);
		num_of_bits[tmp] = in[in_pos++];
//		cout <<(char)tmp << " wartosc = " <<  alphabet_weights[tmp] << " bitow " << num_of_bits[tmp] << endl;
	}

	uint32_t* in32 = reinterpret_cast<uint32_t*>(in + in_pos);

	for(int i = 0; i < data->var_1; ++i){
//		mask = 1 << (7 - i % 8);
		mask = 1 << (31 - i % 32);
		c <<= 1;
//		if((mask & in[in_pos + i/8]) != 0)
		if((mask & in32[i/32]) != 0)
			c += 1;
		bits_num++;
		tmp = findCharacter(c, bits_num);
		if(tmp != -1){
//			cout << "Znak nr = " << out_pos << " bitow = " << bits_num << " znak = " << tmp << endl;
			data->out_buf[out_pos++] = tmp;
			c = 0;
			bits_num = 0;
		}
	}



//	for(int i = 0; i < data->var_1; ++i){
//		tmp = 0;
//		mask = 1 << (7 - i%8);
//		c = c << 1;
//		bits_num++;
//		c += (in[in_pos + i/8] & mask) != 0?1:0;
////		cout << "Mask = " << (int)mask  << " c = " << c << endl;
//		tmp = findCharacter(c, bits_num);
//		if(tmp != -1){
////			cout << "Znak nr = " << out_pos << " bitow = " << bits_num << " znak = " << tmp << endl;
//			data->out_buf[out_pos++] = (uint8_t)tmp;
//			c = 0;
//			bits_num = 0;
//		}
////		if(in_pos + i/8 == 503)
////			cout << "bajt = " << (int)in[in_pos + i/8] <<  " maska = " << (int)mask << " bits_num = " << bits_num << " c = " << (int)c << " tmp = " << tmp << endl;
//
//	}
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
		nodes.push_back(n);
	}

	tree2arrays(nodes.front(), 0, 0, false);
//
//	for(int i = 0; i < alphabet_size; ++i)
//		if(num_of_bits[i] > 0)
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

