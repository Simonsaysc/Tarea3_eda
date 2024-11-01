/*
 * abb.cpp
 *
 *  Created on: Sep 2, 2022
 *      Author: jsaavedr
 */

#include "trees/rb.hpp"
#include <iostream>


namespace trees {

RB::RB():root(nullptr) {
	// TODO Auto-generated constructor stub
}

void RB::balance(RBNode* node){
	RotationType rType = getRotationType(node);
	std::cout << "Rotation Type : "  << rType << std::endl;
	if (rType == RotationType::case_1){
		do_case_1(node);
	}
	else if (rType == RotationType::case_2){
		do_case_2(node);
	}
	else if (rType == RotationType::case_3){
		do_case_3(node);
	}
	else if (rType == RotationType::case_4){
		do_case_4(node);
	}
}

RotationType RB::getRotationType(RBNode* node){
	/*this will run if node is unbalanced*/
	RotationType rType = RotationType::case_1;
	RBNode* leftChild = node->getLeft();
	RBNode* rightChild = node->getRight();
	if(leftChild->getColor()=='R' && rightChild->getColor()=='R'){
		rType = RotationType::case_1;
	}
	else if((leftChild->getColor()=='R' && leftChild->getLeft()->getColor()=='R')||
	rightChild->getColor()=='R' && rightChild->getRight()->getColor()=='R'){
		rType = RotationType::case_2;
	}	
	return rType;
}

 void RB::do_case_1(RBNode* node){
	RBNode* parent = node->getParent();
	RBNode* leftChild = node->getLeft();
	RBNode* rightChild = node->getRight();
	node->setColor(NodeColor::RED);
	if(parent == nullptr){
		node->setColor(NodeColor::BLACK);
	}
	leftChild->setColor(NodeColor::BLACK);
	rightChild->setColor(NodeColor::BLACK);
 }
void RB::do_case_2(RBNode* node){
	RBNode* parent = node->getParent();
	RBNode* leftChild = node->getLeft();
	RBNode* rightChild = node->getRight();
	bool Left = node->isLeft();
	if(leftChild->getColor()=='R' && leftChild->getLeft()->getColor()=='R'){
		node->setLeft(leftChild->getRight());
		leftChild->setRight(node);
		if(node==root){
			root = leftChild;
		}
		else{
			if(Left){
			parent->setLeft(leftChild);
			}
			else{
			parent->setRight(leftChild);
			}
		}
		node->setColor(NodeColor::RED);
		leftChild->setColor(NodeColor::BLACK);
	}
	else if(rightChild->getColor()=='R' && rightChild->getRight()->getColor()=='R'){
		node->setRight(rightChild->getLeft());
		rightChild->setLeft(node);
		if(node==root){
			root = rightChild;
		}
		else{
			if(Left){
			parent->setLeft(rightChild);
			}
			else{
			parent->setRight(rightChild);
			}
		}
		node->setColor(NodeColor::RED);
		rightChild->setColor(NodeColor::BLACK);
	}
}


void RB::insert(int val, RBNode* node){
	RBNode* new_node = new RBNode();
	if (val < node->getData()){
		if (node->getLeft() == nullptr){
			node->setLeft(new RBNode(val, node));
		}
		else{
			insert(val, node->getLeft());
		}
	}
	else{
		if (node->getRight() == nullptr){
			node->setRight(new RBNode(val, node));
		}
		else{
			insert(val, node->getRight());
		}
	}
	
	if(new_node!=nullptr){
		if(new_node->getParent()->isRed()){

		};
	}



}

void RB::insert(int val){
	if (root == nullptr){
		root = new RBNode(val);
	}
	else{
		insert(val, root);
	}
}

RBNode* RB::find(int val, RBNode* node){
	RBNode* ans = nullptr;
	if (node->getData() == val){
		ans = node;
	}
	else{
		if (val < node->getData()){
			find(val, node->getLeft());
		}
		else{
			find(val, node->getRight());
		}
	}
	return ans;
}

RBNode* RB::find(int val){
	RBNode* ans = nullptr;
	ans = find(val, root);
	return ans;
}

void RB::traverse(RBNode* node, int label){
	if (node != nullptr){
		for (int i = 0; i < label; i++){
			std::cout << "*" << std::flush;
		}
		char T = node->isLeft()?'L':'R';
		std::cout << node->getData() << "  " << T <<std::endl;
		traverse(node->getLeft(), label + 1);
		traverse(node->getRight(), label + 1);
	}
}

void RB::traverse(){
	traverse(root, 1);
}


RB::~RB() {
	delete root;
}

} /* namespace trees */
