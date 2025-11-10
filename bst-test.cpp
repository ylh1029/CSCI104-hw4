#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"
#include <random>
#include <set>

using namespace std;

int main(int argc, char *argv[])
{
    // Binary Search Tree tests

    /*
    BinarySearchTree<char,int> bt;
    bt.insert(std::make_pair('a',1));
    bt.insert(std::make_pair('b',2));
    
    cout << "Binary Search Tree contents:" << endl;
    for(BinarySearchTree<char,int>::iterator it = bt.begin(); it != bt.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(bt.find('b') != bt.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    bt.remove('b');

    // AVL Tree Tests
    AVLTree<char,int> at;
    at.insert(std::make_pair('a',1));
    at.insert(std::make_pair('b',2));

    cout << "\nAVLTree contents:" << endl;
    for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
    if(at.find('b') != at.end()) {
        cout << "Found b" << endl;
    }
    else {
        cout << "Did not find b" << endl;
    }
    cout << "Erasing b" << endl;
    at.remove('b');
    */

    /*
    BinarySearchTree<uint16_t, uint16_t> testTree;
    	testTree.insert(std::make_pair(1, 9));
      testTree.insert(std::make_pair(5, 8));
      testTree.insert(std::make_pair(3, 159));
      bool balanced = testTree.isBalanced();
      std::cout << std::boolalpha << balanced << std::endl;
    */

    // AVLTree<uint16_t, uint16_t> testTree;
    // set<uint16_t> inputSet;
    // srand(time(0));

    // for(int i = 0; i < 30; i++){
    //   uint16_t input = rand() % 101;
    //   while (inputSet.find(input) != inputSet.end()){
    //     input = rand() % 101;
    //   }

    //   inputSet.insert(input);
    //   testTree.insert(make_pair(input, 10));
    //   cout << input << " inserted" << endl;
    //   cout << "=========Tree so far=========" << endl;
    //   testTree.BinarySearchTree::print();
    // }

    AVLTree<int, double> bst;
	bst.insert(std::make_pair(2, 1.0));
	bst.insert(std::make_pair(3, 1.0));
	bst.insert(std::make_pair(1, 1.0));
	bst.insert(std::make_pair(4, 1.0));
	bst.remove(1);
    bst.BinarySearchTree::print();

    return 0;
}
