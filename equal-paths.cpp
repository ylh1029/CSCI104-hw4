#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root){
    if(!root){
        return 0;
    }

    else if(!(root->left) && !(root->right)){
        return 1;
    }

    else if(!(root->left)){
        return height(root->right) + 1;
    }

    else if(!(root->right)){
        return height(root->left) + 1;
    }

    else{
        return height(root->left) + height(root->right) + 1;
    }
}


bool equalPaths(Node * root)
{
    // Add your code below
    if(!root){
        return true;
    }

    else if(!(root -> left) && !(root->right)){
        return true;
    }

    else if (root->left && root->right){
        int lheight = height(root->left);
        int rheight = height(root->right);

        return lheight == rheight && equalPaths(root->left) && equalPaths(root->right);
    }

    else{
        return true;
    }

}

