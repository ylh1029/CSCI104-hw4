#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
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
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
private: 
    void insert_fix( AVLNode<Key,Value>* parent, AVLNode<Key,Value>* curr);
    void rotateRight(AVLNode<Key, Value>* target);
    void rotateLeft(AVLNode<Key, Value>* target);
    void removeFix(AVLNode<Key, Value>* curr, int diff);
    AVLNode<Key, Value>* getRoot() const;
    void setRoot(AVLNode<Key, Value>* value);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key, Value>* temp = getRoot();
    bool found = false;
    Key target_key = new_item.first;

    if(getRoot() == nullptr){
        AVLNode<Key, Value>* add = new AVLNode<Key, Value>(new_item.first, new_item.second, nullptr);
        setRoot(add);
        add->setBalance(0);
        found = true;
        return;
    }

    while (!found){
        if(temp->Node<Key, Value>::getKey() > target_key){
            if(temp->getLeft() == nullptr){
                Node<Key, Value>* add = new Node<Key, Value>(new_item.first, new_item.second, temp);
                temp -> setLeft(add);
                found = true;
            }

            else{
                temp = temp -> getLeft();
            }
        }

        else if(temp->Node<Key, Value>::getKey() < target_key){
            if(temp -> getRight() == nullptr){
                Node<Key, Value>* add = new Node<Key, Value>(new_item.first, new_item.second, temp);
                temp -> setRight(add);
                found = true;
            }

            else{
                temp = temp -> getRight();
            }
        }

        else{
            found = true;
            temp -> setValue(new_item.second);
        }
    }

    temp -> setBalance(0);

    if(temp -> getParent() -> getBalance() == -1){
        temp -> getParent() -> setBalance(0);
    }

    else if(temp -> getParent() -> getBalance() == 1){
        temp -> getParent() -> setBalance(0);
    }

    else{
        if(temp->getParent()->getLeft() && temp->getParent()->getLeft() == temp){
        //temp is the left child
            temp -> getParent() -> setBalance(1); 
        }

        else{
        //temp is the right child
            temp -> getParent() -> setBalance(-1);
        }

        insert_fix(temp->getParent(), temp);
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    bool found = false;
    AVLNode<Key, Value>* temp = getRoot();
    AVLNode<Key, Value>* parent;
    int diff;
    bool right = true;

    while(!found && temp != nullptr){
    //This while loop finds the value and removes it: 
    //Number of child is considered 
        if(temp -> Node<Key, Value>::getKey() > key){
            temp = temp -> getLeft();
        }

        else if(temp -> Node<Key, Value>::getKey() < key){
            temp = temp -> getRight();
        }

        else{
        //Key for removal was found
            if(temp -> getLeft() == nullptr && temp -> Node<Key, Value>::getRight() == nullptr){
            //Zero children
              if(temp == getRoot()){
                setRoot(nullptr);
              }

              else{
                if(temp->getParent()->getLeft() == temp){
                //temp is a left child
                  temp->getParent()->setLeft(nullptr);
                  right = false;
                }

                else{
                //temp is a right child
                  temp->getParent()->setRight(nullptr);
                }
              }

              parent = temp -> getParent();
              delete temp;
            }

            else if(temp -> getLeft() && temp -> getRight()){
            //Two children
                //Swap the predecessor with the current Node
                    AVLNode<Key, Value>* pred = AVLTree<Key, Value>::predecessor(temp);
                    nodeSwap(temp, pred);

                    if(temp->getParent()){
                    //temp has a parent
                      if(temp->getParent()->getLeft() == temp && temp->getLeft()){
                      ///Temp is a left child and has a left child
                        temp->getParent()->setLeft(temp->getLeft());
                        temp->getLeft()->setParent(temp->getParent());
                        right = false;
                      }

                      else if(temp->getParent()->getLeft() == temp){
                      //Temp is a left child and has 0 child
                        temp->getParent()->setLeft(nullptr);
                        right = false;
                      }

                      else if(temp->getParent()->getRight() == temp && temp->getLeft()){
                        //Temp is a right child and has a left child
                        temp->getParent()->setRight(temp->getLeft());
                        temp->getLeft()->setParent(temp->getParent());
                      }

                      else if(temp->getParent()->getRight() == temp){
                        //Temp is a right child and has 0 child
                        temp->getParent()->setRight(nullptr);
                      }
                    }

                    parent = temp -> getParent();
                    delete temp;
            }

            else{
            //One child
                AVLNode<Key, Value>* child;
                if(temp->getRight()){
                //Temp has right child only
                    temp->getRight()->setParent(temp->getParent());
                    child = temp->getRight();
                }

                else{
                //Temp has left child only
                    temp->getLeft()->setParent(temp->getParent());
                    child = temp->getLeft();
                }

                if(temp == getRoot()){
                //If temp had no parent(): root
                  setRoot(child);
                }

                else if(temp->getParent()->getRight() == temp){
                //If temp was the right child of its parent
                    temp->getParent()->setRight(child);
                }

                else{
                //If temp was the left child of its parent
                    temp->getParent()->setLeft(child);
                    right = false;
                }

                parent = temp -> getParent();
                delete temp;
            }
            found = true;
        }
    }

    if(right && parent){
        diff = -1;
    }

    else if(!right && parent){
        diff = 1;
    }

    removeFix(parent, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert_fix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* current){
    if(!parent || !current){
        return;
    }

    AVLNode<Key, Value>* grandParent = parent -> getParent();

    if(!grandParent){
        return;
    }

    if(grandParent -> getLeft() && grandParent -> getLeft() == parent){
    //Parent is the left child
        grandParent -> setBalance(grandParent -> getBalance() - 1);
        if(grandParent -> getBalance() == 0){
            return;
        }

        else if(grandParent -> getBalance() == -1){
            insert_fix(grandParent, parent);
        }

        else{
        //Balance is -2
            if(parent -> getLeft() && parent -> getLeft() == current){
            //zig-zig: current is the left child of parent
                rotateRight(grandParent);
                parent -> setBalance(0);
                grandParent -> setBalance(0);
            }

            else{
            //zig-zag: current is the right child of parent
                rotateLeft(parent);
                rotateRight(grandParent);

                if(current -> getBalance() == -1){
                //Balance of n is -1
                    current -> setBalance(0);
                    parent -> setBalance(0);
                    grandParent -> setBalance(1);
                }

                else if(current -> getBalance() == 0){
                //Balance of n is 0
                    current -> setBalance(0);
                    parent -> setBalance(0);
                    grandParent -> setBalance(0);
                }

                else{
                //Balance of n is 1
                    current -> setBalance(0);
                    grandParent -> setBalance(0);
                    parent -> setBalance(-1);
                }
            }
        }
    }

    else{
    //Parent is the right child
        grandParent -> setBalance(grandParent -> getBalance() + 1);
        if(grandParent -> getBalance() == 0){
            return;
        }

        else if(grandParent -> getBalance() == 1){
            insert_fix(grandParent, parent);
        }

        else{
            if(parent -> getRight() == current){
            //Zig zig
                rotateLeft(grandParent);
                parent -> setBalance(0);
                grandParent -> setBalance(0);
            }

            else{
            //Zig zag
                rotateRight(parent);
                rotateLeft(grandParent);
                
                if(current -> getBalance() == 1){
                    parent -> setBalance(0);
                    grandParent -> setBalance(-1);
                    current -> setBalance(0);
                }

                else if(current -> getBalance() == 0){
                    parent -> setBalance(0);
                    grandParent -> setBalance(0);
                    current -> setBalance(0);
                }

                else{
                    parent -> setBalance(-1);
                    grandParent -> setBalance(0);
                    current -> setBalance(0);
                }
            }
        }
    }

}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* target){
//Rotate right means right heavy
    if(target -> getRight() -> getLeft() && target -> getRight() -> getRight()){
    //Target_child has two children
        AVLNode<Key, Value>* left_child = target -> getRight() -> getLeft();
        target -> getRight() -> setLeft(target);
        target -> getRight() -> setParent(target->getParent());
        target -> setParent(target -> getRight());
        target -> setRight(nullptr);
        target -> getParent() -> getRight() -> setLeft(left_child);
        target -> getParent() -> getRight() -> getLeft() -> setParent(target -> getParent() -> getRight());
    }

    else{
    //Target_child has one child
        target -> getRight() -> setLeft(target);
        target -> getRight() -> setParent(target -> getParent());
        target -> setParent(target -> getRight());
        target -> setRight(nullptr);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* target){
//Rotate right means left heavy
    if(target -> getLeft() -> getLeft() && target -> getLeft() -> getRight()){
    //Target_child has two children
        AVLNode<Key, Value>* right_child = target -> getLeft() -> getRight();
        target -> getLeft() -> setRight(target);
        target -> getLeft() -> setParent(target->getParent());
        target -> setParent(target -> getLeft());
        target -> setLeft(nullptr);
        target -> getParent() -> getLeft() -> setRight(right_child);
        target -> getParent() -> getLeft() -> getRight() -> setParent(target -> getParent() -> getLeft());
    }

    else{
    //Target_child has one child
        target -> getLeft() -> setRight(target);
        target -> getLeft() -> setParent(target -> getParent());
        target -> setParent(target -> getLeft());
        target -> setLeft(nullptr);
    }
}

template <class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* curr, int diff){
    if(!curr){
        return;
    }

    int ndiff = 0;

    AVLNode<Key, Value>* parent = curr -> getParent();
    
    if(parent && parent -> getLeft() == curr){
    //Curr is the left child
        ndiff = 1;
    }

    else if(parent && parent -> getRight() == curr){
    //Curr is the right child
        ndiff = -1;
    }

    if(diff == 1){

    }

    else{
    //diff = -1
        if(curr -> getBalance() + diff == -2){
            AVLNode<Key, Value>* child = curr -> getLeft();

            if(child -> getBalance() == -1){
            //Zig-zig case
                rotateRight(curr);
                curr -> setBalance(0);
                child -> setBalance(0);
                removeFix(parent, ndiff);
            }

            else if(child -> getBalance() == 0){
            //Zig-zig case
                rotateRight(curr);
                curr -> setBalance(-1);
                child -> setBalance(1);
            }

            else{
            //Zig-zag case
                AVLNode<Key, Value>* grandParent = child -> getRight();
                rotateLeft(child);
                rotateRight(curr);
                
                if(grandParent -> getBalance() == 1){
                    curr -> setBalance(0);
                    child -> setBalance(-1);
                    grandParent -> setBalance(0);
                }

                else if(grandParent -> getBalance() == 0){
                    curr -> setBalance(0);
                    child -> setBalance(0);
                    grandParent -> setBalance(0);
                }

                else{
                    curr -> setBalance(1);
                    child -> setBalance(0);
                    grandParent -> setBalance(0);
                }
            }
        }

        else if(curr -> getBalance() + diff == -1){
            curr -> setBalance(-1);
        }

        else{
            curr -> setBalance(0);
            removeFix(parent, ndiff);
        }
    }
}

template <class Key, class Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::getRoot() const{
    return static_cast<AVLNode<Key, Value>*>(BinarySearchTree<Key, Value>::getRoot());
}

template <class Key, class Value>
void AVLTree<Key, Value>::setRoot(AVLNode<Key, Value>* value){
    if(!value){
        BinarySearchTree<Key, Value>::setRoot(nullptr);
    }
    else{
        BinarySearchTree<Key, Value>::setRoot(static_cast<Node<Key, Value>*>(value));
    }
}


#endif
