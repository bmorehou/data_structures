#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <cstdlib>
#include <string>
#include "bst.h"

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus 
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
	// Constructor/destructor.
	AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
	virtual ~AVLNode();

	// Getter/setter for the node's height.
	int getHeight() const;
	void setHeight(int height);

	// Getters for parent, left, and right. These need to be redefined since they 
	// return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
	// for more information.
	virtual AVLNode<Key, Value>* getParent() const override;
	virtual AVLNode<Key, Value>* getLeft() const override;
	virtual AVLNode<Key, Value>* getRight() const override;

protected:
	int mHeight;
	
};

/*
--------------------------------------------
Begin implementations for the AVLNode class.
--------------------------------------------
*/

/**
* Constructor for an AVLNode. Nodes are initialized with a height of 0.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent)
	: Node<Key, Value>(key, value, parent)
	, mHeight(0)
{

}

/**
* Destructor.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* Getter function for the height. 
*/
template<typename Key, typename Value>
int AVLNode<Key, Value>::getHeight() const
{
	return mHeight;
}

/**
* Setter function for the height. 
*/
template<typename Key, typename Value>
void AVLNode<Key, Value>::setHeight(int height)
{
	mHeight = height;
}

/**
* Getter function for the parent. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getParent() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mParent);
}

/**
* Getter function for the left child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getLeft() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mLeft);
}

/**
* Getter function for the right child. Used since the node inherits from a base node.
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLNode<Key, Value>::getRight() const
{
	return static_cast<AVLNode<Key,Value>*>(this->mRight);
}

/*
------------------------------------------
End implementations for the AVLNode class.
------------------------------------------
*/

/**
* A templated balanced binary search tree implemented as an AVL tree.
*/
template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
	// Methods for inserting/removing elements from the tree. You must implement
	// both of these methods. 
	virtual void insert(const std::pair<Key, Value>& keyValuePair) override;
	void remove(const Key& key);
	void print()const override; 
	void printHelper(AVLNode<Key,Value>*)const;

private:
	/* Helper functions are strongly encouraged to help separate the problem
	   into smaller pieces. You should not need additional data members. */
	AVLNode<Key, Value>* BSTInsertion(const std::pair<Key,Value>& keyValuePair);
	void SetHeightHelper(AVLNode<Key,Value>*);
	void DetermineRotation(AVLNode<Key,Value>*,AVLNode<Key,Value>*,AVLNode<Key,Value>*);
	void LeftLeft(AVLNode<Key,Value>*,AVLNode<Key,Value>*,AVLNode<Key,Value>*);
	void RightLeft(AVLNode<Key,Value>*,AVLNode<Key,Value>*,AVLNode<Key,Value>*);
	void LeftRight(AVLNode<Key,Value>*,AVLNode<Key,Value>*,AVLNode<Key,Value>*);
	void RightRight(AVLNode<Key,Value>*,AVLNode<Key,Value>*,AVLNode<Key,Value>*);
};

/*
--------------------------------------------
Begin implementations for the AVLTree class.
--------------------------------------------
*/

/**
* Insert function for a key value pair. Finds location to insert the node and then balances the tree. 
*/
template<typename Key, typename Value>
void AVLTree<Key, Value>::insert(const std::pair<Key, Value>& keyValuePair)
{
	/*
	 *	 psuedocode
	 *
	 *	 1. insert into binary search tree.
	 *	 2. recurse up and update heights as we go
	 *	 	check to make sure node heights never differ by more than one.
	 *	 	If they do have an inbalance, then you need to rotate accordingly.
	 *	 	After you rotate, update and ensure that they are balanced.
	 *	 
	 */	
	AVLNode<Key,Value> *currentNode = this->BSTInsertion(keyValuePair); // first we do some basic insertion!
	if(currentNode == nullptr){
		return;
	}
	currentNode->setHeight(1);
	bool balanced = true;
	while(balanced){
		if(currentNode == nullptr){
			std::cout<<"Broke loop"<<std::endl;
			break;
		}
		this->SetHeightHelper(currentNode);// sets the height as it goes up.
		std::cout<<"currentNode is"<<currentNode->getKey()<<std::endl;	
		std::cout<<"currentNode height is"<<currentNode->getHeight()<<std::endl;	
		if(currentNode->getLeft() == nullptr && currentNode->getRight() == nullptr){ 
			currentNode = currentNode->getParent();	
			continue;
		}else if(currentNode->getRight() != nullptr && currentNode->getLeft() == nullptr){
			if(currentNode->getRight()->getHeight() > 1){ // we are not updating heights as expected!
				AVLNode<Key,Value> *zNode = currentNode; // first we do some basic insertion!
				AVLNode<Key,Value> *yNode = currentNode->getRight(); // first we do some basic insertion!
				AVLNode<Key,Value> *xNode = nullptr; // first we do some basic insertion!
				if(keyValuePair.first < yNode->getKey()){
					xNode = yNode->getLeft(); // first we do some basic insertion!
				}else{
					xNode = yNode->getRight(); // first we do some basic insertion!
				}
				this->DetermineRotation(zNode,yNode,xNode);		// seg fault somewhere in here.
				return;
			}else{
				currentNode->setHeight(currentNode->getRight()->getHeight()+1);	
				currentNode = currentNode->getParent();	
			}
		}else if(currentNode->getLeft() != nullptr && currentNode->getRight() == nullptr){
			if(currentNode->getLeft()->getHeight() > 1){
				AVLNode<Key,Value> *zNode = currentNode; // first we do some basic insertion!
				AVLNode<Key,Value> *yNode = currentNode->getLeft(); // first we do some basic insertion!
				AVLNode<Key,Value> *xNode = nullptr; // first we do some basic insertion!
				if(keyValuePair.first < yNode->getKey()){
					xNode = yNode->getLeft(); // first we do some basic insertion!
				}else{
					xNode = yNode->getRight(); // first we do some basic insertion!
				}
				this->DetermineRotation(zNode,yNode,xNode);		
				return;
			}else{
				currentNode->setHeight(currentNode->getLeft()->getHeight()+1);
				currentNode = currentNode->getParent();	
			}
		}else if(std::abs(currentNode->getLeft()->getHeight() - currentNode->getRight()->getHeight()) > 1){
			// then it is unbalanced at this node and this is where we need to start rotating!
			AVLNode<Key,Value> *zNode = currentNode; // first we do some basic insertion!
			AVLNode<Key,Value> *yNode = nullptr; // first we do some basic insertion!
			AVLNode<Key,Value> *xNode = nullptr; // first we do some basic insertion!
			if(keyValuePair.first < currentNode->getKey()){
				yNode = currentNode->getLeft(); // first we do some basic insertion!
			}else{
				yNode = currentNode->getRight(); // first we do some basic insertion!
			}
			if(keyValuePair.first < yNode->getKey()){
				xNode = yNode->getLeft(); // first we do some basic insertion!
			}else{
				xNode = yNode->getRight(); // first we do some basic insertion!
			}
			// now we figure out which rotation we need to do.
			this->DetermineRotation(zNode,yNode,xNode);		
			return;
		}else{
			if(currentNode == static_cast<AVLNode<Key,Value>*>(this->mRoot)){
				break;
			}else{
				currentNode = currentNode->getParent();
				continue;
			}
		}
	}
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::SetHeightHelper(AVLNode<Key,Value>*currentNode){
	if(currentNode == nullptr){
		return;
	}else if(currentNode->getRight() == nullptr && currentNode->getLeft() == nullptr){
		currentNode->setHeight(1);
		return;
	}else if(currentNode->getRight() != nullptr && currentNode->getLeft() == nullptr){
		currentNode->setHeight(currentNode->getRight()->getHeight()+1);
		return;
	}else if(currentNode->getRight() == nullptr && currentNode->getLeft() != nullptr){
		currentNode->setHeight(currentNode->getLeft()->getHeight()+1);
		return;
	}else{
		int newHeight = std::max(currentNode->getRight()->getHeight(),currentNode->getLeft()->getHeight());
		currentNode->setHeight(newHeight+1);
	}
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key,Value>::BSTInsertion(const std::pair<Key,Value>& keyValuePair){
	if(static_cast<AVLNode<Key,Value>*>(this->mRoot) == nullptr){
		AVLNode<Key,Value> *newRootNode = new AVLNode<Key,Value>(keyValuePair.first,keyValuePair.second,nullptr); // we make the node and dynamically allocate it here.
		newRootNode->setHeight(1);
		this->mRoot = newRootNode;
		return nullptr;
	}
	AVLNode<Key, Value> *currentNode = static_cast<AVLNode<Key,Value>*>(this->mRoot);
	// we want to use the iterator and go down the tree
	bool exit = false;
	while(!exit){
		if(keyValuePair.first < currentNode->getKey()){
			if(currentNode->getLeft() == nullptr){
				AVLNode<Key,Value> *newNode = new AVLNode<Key,Value>(keyValuePair.first,keyValuePair.second,currentNode); 
				currentNode->setLeft(newNode);
				return newNode;
			}else{
				currentNode = currentNode->getLeft();
				continue;
			}
		}else if(keyValuePair.first > currentNode->getKey()){
			if(currentNode->getRight() == nullptr){
				AVLNode<Key,Value> *newNode = new AVLNode<Key,Value>(keyValuePair.first,keyValuePair.second,currentNode); 
				currentNode->setRight(newNode);
				return newNode;
			}else{
				currentNode = currentNode->getRight();
				continue;
			}
		}else{
			currentNode->setValue(keyValuePair.second);
			return nullptr;
		}
	}
	// if it gets to here then this is a bad data structure and I will exit the program.
	std::cout<<"Binary Search tree insert is broken"<<std::endl;
	std::exit(1);
}

/* 
 * Each one of these four is to help with the rotations that are neccessary for add and remove...
 */

template<typename Key, typename Value>
void AVLTree<Key,Value>::DetermineRotation(AVLNode<Key,Value> *zNode,AVLNode<Key,Value> *yNode,AVLNode<Key,Value> *xNode){
	if(zNode->getLeft() == yNode && yNode->getLeft() == xNode){
		this->LeftLeft(zNode,yNode,xNode);		
	}else if(zNode->getLeft() == yNode && yNode->getRight() == xNode){
		this->LeftRight(zNode,yNode,xNode);		
	}else if(zNode->getRight() == yNode && yNode->getRight() == xNode){
		this->RightRight(zNode,yNode,xNode);		
	}else{
		this->RightLeft(zNode,yNode,xNode);		
	}
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::LeftLeft(AVLNode<Key,Value> *zNode,AVLNode<Key,Value> *yNode,AVLNode<Key,Value> *xNode){
	AVLNode<Key,Value> *T3 = yNode->getRight();
	if(zNode->getParent() != nullptr){	
		if(zNode == zNode->getParent()->getRight()){
			zNode->getParent()->setRight(yNode);
		}else{
			zNode->getParent()->setLeft(yNode);
		}
	}else{
		this->mRoot = yNode;
	}
	zNode->setParent(yNode);
	zNode->setLeft(T3);
	yNode->setRight(zNode);
	if(T3 != nullptr){
		T3->setParent(zNode);
	}

	// now let's update heights.
	this->SetHeightHelper(xNode);
	this->SetHeightHelper(zNode);
	this->SetHeightHelper(yNode);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::RightLeft(AVLNode<Key,Value> *zNode,AVLNode<Key,Value> *yNode,AVLNode<Key,Value> *xNode){ // check this 
	AVLNode<Key,Value> *T3 = xNode->getRight();
	zNode->setRight(xNode);
	xNode->setParent(zNode);
	yNode->setParent(xNode);
	xNode->setRight(yNode);
	yNode->setLeft(T3);
	T3->setParent(yNode);
	this->LeftLeft(zNode,xNode,yNode);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::LeftRight(AVLNode<Key,Value> *zNode,AVLNode<Key,Value> *yNode,AVLNode<Key,Value> *xNode){ // check this
	AVLNode<Key,Value> *T2 = xNode->getRight();
	zNode->setLeft(xNode);
	xNode->setParent(zNode);
	xNode->setLeft(yNode);
	yNode->setParent(xNode);
	yNode->setRight(T2);
	T2->setParent(yNode);
	this->RightRight(zNode,xNode,yNode);
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::RightRight(AVLNode<Key,Value> *zNode,AVLNode<Key,Value> *yNode,AVLNode<Key,Value> *xNode){
	std::cout<<"test"<<std::endl;
	
	AVLNode<Key,Value> *T2 = yNode->getLeft();
	if(zNode->getParent() != nullptr){	
		if(zNode == zNode->getParent()->getRight()){
			zNode->getParent()->setRight(yNode);
		}else{
			zNode->getParent()->setLeft(yNode);
		}
	}else{
		this->mRoot = yNode; // this means znode was root before but now y is.
	}

	yNode->setParent(zNode->getParent());
	zNode->setParent(yNode);
	zNode->setRight(T2);
	yNode->setLeft(zNode);
	if(T2 != nullptr){
		T2->setParent(zNode);
	}
	
	this->SetHeightHelper(xNode);
	this->SetHeightHelper(zNode);
	this->SetHeightHelper(yNode);
	std::cout<<"z node is:"<<zNode->getValue()<<std::endl;
	std::cout<<"z node height is:"<<zNode->getHeight()<<std::endl;
	std::cout<<"y node is:"<<yNode->getValue()<<std::endl;
	std::cout<<"y node height is:"<<yNode->getHeight()<<std::endl;
	std::cout<<"x node is:"<<xNode->getValue()<<std::endl;
	std::cout<<"x node height is:"<<xNode->getHeight()<<std::endl;
}

/**
* Remove function for a given key. Finds the node, reattaches pointers, and then balances when finished. 
*/

template<typename Key, typename Value>
void AVLTree<Key, Value>::print() const
{
	/*
	printRoot(mRoot);
	std::cout << "\n";
	*/
	AVLNode<Key, Value> *currentNode = static_cast<AVLNode<Key,Value>*>(this->mRoot);
	std::cout<<"\nAVL PRINT\n***********************************"<<std::endl;
	printHelper(currentNode);
	std::cout<<"***********************************\n"<<std::endl;
}

template<typename Key, typename Value>
void AVLTree<Key,Value>::printHelper(AVLNode<Key,Value>* currentNode) const{
	if(currentNode == nullptr){
		return;
	}
	this->printHelper(currentNode->getLeft());
	if(currentNode == static_cast<AVLNode<Key,Value>*>(this->mRoot)){std::cout<<"The root is:";}
	std::cout<<currentNode->getItem().first<<std::endl;
	std::cout<<"height is"<<currentNode->getHeight()<<std::endl;
	this->printHelper(currentNode->getRight());
}

template<typename Key, typename Value>
void AVLTree<Key, Value>::remove(const Key& key)
{

}

/*
------------------------------------------
End implementations for the AVLTree class.
------------------------------------------
*/

#endif
