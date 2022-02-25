//
// Created by kianr on 4/11/2021.
//

/**
 * This AVL Tree will be used mainly for getting the words.
 * Credit can be given to Professor Fontenot for the in class explanation
 */

#ifndef SEARCH_ENGINE_AVLTREE_H
#define SEARCH_ENGINE_AVLTREE_H

#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

template<typename T>
class AVLTree {
private:
    template<typename K>
    struct avlNode {
        T data;
        avlNode *left;
        avlNode *right;
        int height;

        avlNode(const T & theElement,
                avlNode *lt,
                avlNode *rt,
                int h = 0) :
                data(theElement),
                left(lt),
                right(rt),
                height(h) {}

        /*
               avlNode(T d) {
                   height = 1;
                   data = d;
                   left = nullptr;
                   right = nullptr;
               }

               avlNode(T d, avlNode* l, avlNode* r) {
                   //height
                   data = d;
                   left = l;
                   right = r;
               }
       */
        //copy constructor
        avlNode(const avlNode &copy) {
            data = copy.data;
            left = copy.left;
            right = copy.right;
            height = copy.height;
        }

        //destructor
        ~avlNode(){

        }

        //equal operator
        avlNode &operator= (const avlNode &copy) {
            data = copy.data;
            left = copy.left;
            right = copy.right;
            height = copy.height;
        }

        //get functions
        T getData() {
            return data;
        }
        T getHeight() {
            return height;
        }
    };


    avlNode<T>* root;
    int count = 0;  //in case


    void printTree(avlNode<T>* node) const{
        if(node != nullptr) {
            printTree(node->left);
            cout << node->data << endl;
            printTree(node->right);
        }
    }




public:
    AVLTree() : root(NULL) {}
    AVLTree(const AVLTree& rhs) : root(NULL) {*this = rhs;}
    ~AVLTree() {
        makeEmpty();
    }

    void makeEmpty() {
        makeEmpty(root);
        count--;
    }
    void makeEmpty(avlNode<T>*& node) {
        // if(node != nullptr) {
        if(node->left != nullptr)
            makeEmpty(node->left);
        if(node->right != nullptr)
            makeEmpty(node->right);
        delete node;
        //      count--;
        // }
        node = nullptr;
        count--;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    int height(avlNode<T> *node) {
        if(node == nullptr)
            return -1;
        return node->height;
    }

    void insert(const T&x) {
        insert(x, root);
        count++;
    }

    void insert(const T& x, avlNode<T>*& t) {
        if(t == nullptr)
            t = new avlNode<T>(x, NULL, NULL);
        else if(x > t->data) {
            insert(x, t->right);
            if(height(t->right) - height(t->left) > 1) {
                if(t->right->data < x) {
                    rotateWithRightChild(t);
                }
                else {
                    doubleWithRightChild(t);
                }
            }
        }
        else if(x < t->data) {
            insert(x, t->left);
            if(height(t->left) - height(t->right) > 1) {
                if(x < t->left->data) {
                    rotateWithLeftChild(t);
                }
                else {
                    doubleWithLeftChild(t);
                }
            }
        }
        //balance(t);

        t->height = max(height(t->left), height(t->right)) + 1;
        // count++;
    }

    void rotateWithLeftChild(avlNode<T>*& k2) {
        avlNode<T>* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;

        k2 = k1;
    }

    void rotateWithRightChild(avlNode<T>*& k2) {
        avlNode<T>* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->right), k2->height) + 1;

        k2 = k1;
    }

    void doubleWithLeftChild(avlNode<T>*& k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    void doubleWithRightChild(avlNode<T>*& k3) {
        rotateWithLeftChild(k3->right);
        rotateWithRightChild(k3);
    }


    /*  void balance(avlNode*& t) {
        if(height(t->left) - height(t->right) > 1) {
            if (height(t->left->left) < -height(t->left->right))
                rotateWithLeftChild(t); //C1
            else
                doubleWithLeftChild(t);
        }
        else if(height(t->right) - height(t->left) > 1) {
            if (height(t->right->left) >= height(t->right->right))
                doubleWithRightChild(t);
            else
                rotateWithRightChild(t);
        }
        t->height = max(height(t->left), height(t->right)) + 1;
    }
*/



    avlNode<T>* find(T x, avlNode<T>* node) const {
        avlNode<T>* temp = nullptr;

        //if x < node...call function again using left of node
        if(x < node->data) {
            if(node->left != nullptr)
                temp = find(x, node->left);
        }
            //if x > node...call function again using right of node
        else if(x > node->data) {
            if(node->right != nullptr)
                temp = find(x, node->right);
        }
            //else if equal...return
        else if(x == node->data)
            temp = node;

        return temp;
    }

    //just try to find the data
    bool find(T x) {
        if(root == nullptr)
            return false;
        else {
            avlNode<T>* node = find(x, root);
            if(node == nullptr)
                return false;
            else
                return true;
        }
    };

    //return data of node with found data
    T& get(T x) {
        avlNode<T>* temp = find(x, root);
        return temp->data;
    };

    void print() const{
        avlNode<T>* node = root;
        if(node != nullptr) {
            printTree(node);
        }
    };

    int getCount() {
        return count;
    }



    vector<T> vec;
    void makeVector(avlNode<T> *node) {
        if(node->left != nullptr)
            makeVector(node->left);
        if(node->right != nullptr)
            makeVector(node->right);
        vec.push_back(node->data);
    }

    vector<T> getVec() {
        makeVector(root);
        return vec;
    }


};


#endif //SEARCH_ENGINE_AVLTREE_H
