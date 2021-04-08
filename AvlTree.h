/*
 * Prevzaté z https://github.com/Travelinglight/AVLTree/blob/master/AVLTree.h
 *
 * Taken from https://github.com/Travelinglight/AVLTree/blob/master/AVLTree.h
 */

/*
AVLTree.h
Copyright (C) 2015-2019 Kingston Chan
This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this code.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.
3. This notice may not be removed or altered from any source distribution.
Kingston Chan
*/

#ifndef AVLTREE_H
#define AVLTREE_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

class NULLT {};
class AVLERR {
public :
    std::string error;
    AVLERR();
    AVLERR(std::string info) {
        error = info;
    }
};

inline int MAX(int a, int b) {
    return a > b ? a : b;
}

template<typename T1>
int dCmp(const T1 &a, const T1 &b) {
    if (a > b)
        return 1;
    if (a < b)
        return -1;
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////Tree node/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2 = NULLT>
class Node {

private:
    T1 ID;
    T2 *Rcd;	// record
    Node *Lft, *Rgt;
    int height;

public:
    // constructor and destructor
    Node();
    Node(const T1 &id, const T2 * const rcd = NULL);
    Node(const T1 &id, const T2 &rcd);
    Node(const Node<T1, T2> &New);
    ~Node();

    // modify the info of private members
    bool ModifyID(const T1 &tmp);
    bool ModifyHeight(int h);
    bool operator=(const Node<T1, T2> &b);
    bool operator=(const T1 &id);
    bool copy(const Node<T1, T2> * const b);
    bool AddLft(Node<T1, T2> *lft);
    bool AddRgt(Node<T1, T2> *rgt);
    bool AddLft(const T1 &lftID, const T2 * const lftRcd = NULL);
    bool AddRgt(const T1 &rgtID, const T2 * const RgtRcd = NULL);

    // get the info of private members
    Node<T1, T2> *getLft() const { return Lft; }
    Node<T1, T2> *getRgt() const { return Rgt; }
    int getHeight() const { return height; }
    const T1 &getID() const { return ID; }
    T2 *getRcd() const { return Rcd; }
    void print() const;
};

////////////////////////////////////////////////////////////////////////////////
//        NAME: Node
// DESCRIPTION: Constructor of Node class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: height, Rcd, Lft, Rgt
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>::Node() {
    height = 0;
    Rcd = new T2;
    Lft = Rgt = NULL;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Node
// DESCRIPTION: Constructor of Node class.
//   ARGUMENTS: const T1 &id - the ID of the node
//				const T2 * const rcd - the initial record with default value NULL
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, height, Lft, Rgt
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-09
//							KC 2015-02-09
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>::Node(const T1 &id, const T2 * const rcd) {
    ID = id;
    Rcd = new T2;
    if (rcd != NULL)
        *Rcd = *rcd;
    Lft = Rgt = NULL;	// no sons at first
    height = 0;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Node
// DESCRIPTION: Constructor of Node class.
//   ARGUMENTS: const T1 &id - the ID of the node
//				const T2 &rcd - the initial record
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, height, Lft, Rgt
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-09
//							KC 2015-02-09
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>::Node(const T1 &id, const T2 &rcd) {
    ID = id;
    Rcd = new T2;
    *Rcd = rcd;
    Lft = Rgt = NULL;	// no sons at first
    height = 0;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Node
// DESCRIPTION: Copy constructor of Node class.
//   ARGUMENTS: const Node<T1, T2> &New - the Node that is to be copied
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, height, Lft, Rgt
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>::Node(const Node<T1, T2> &New) {
    copy(&New);
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ~Node
// DESCRIPTION: Destructor of Node class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, height, Lft, Rgt
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>::~Node() {
    if (Rcd != NULL)
        delete Rcd;
    if (Lft != NULL)
        delete Lft;
    if (Rgt != NULL)
        delete Rgt;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ModifyID
// DESCRIPTION: To modify the ID of a Node.
//   ARGUMENTS: const T1 &tmp - the new ID value
// USES GLOBAL: none
// MODIFIES GL: ID
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::ModifyID(const T1 &tmp) {
    ID = tmp;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: ModifyHeight
// DESCRIPTION: To modify the height of a Node.
//   ARGUMENTS: int h - the new height value
// USES GLOBAL: none
// MODIFIES GL: height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-09
//							KC 2015-02-09
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::ModifyHeight(int h) {
    height = h;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: copy
// DESCRIPTION: To copy the node and their sons.
//   ARGUMENTS: const Node<T1, T2> * const b - the new node that is to be copied
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, Lft, Rgt, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-08
//							KC 2015-02-08
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::copy(const Node<T1, T2> * const b) {

    // avoid self copy after deletion
    if (b == this)
        return true;

    // copy ID, record and height
    ID = b->ID;
    if (b->Rcd != NULL) {
        if (Rcd == NULL)
            Rcd = new T2;
        *Rcd = *(b->Rcd);
    }
    else {
        if (Rcd != NULL) {
            delete Rcd;
            Rcd = NULL;
        }
    }
    height = b->height;

    // copy the left son
    if (b->Lft != NULL) {
        if (Lft == NULL)
            Lft = new Node<T1, T2>;
        Lft->copy(b->Lft);
    }
    else {
        if (Lft != NULL) {
            delete Lft;
            Lft = NULL;
        }
    }

    // copy the right son
    if (b->Rgt != NULL) {
        if (Rgt == NULL)
            Rgt = new Node<T1, T2>;
        Rgt->copy(b->Rgt);
    }
    else {
        if (Rgt != NULL) {
            delete Rgt;
            Rgt = NULL;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: operator=
// DESCRIPTION: copy the content of a Node.
//   ARGUMENTS: const Node<T1, T2> &b - the Node that is to be assigned
// USES GLOBAL: none
// MODIFIES GL: ID, Rcd, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-09
//							KC 2015-02-09
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::operator=(const Node<T1, T2> &b) {
    if (&b == this)
        return true;
    ID = b.getID();
    *Rcd = *(b.getRcd());
    height = b.getHeight();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: operator=
// DESCRIPTION: Assign the ID of a Node.
//   ARGUMENTS: const T1 &id - the id that is to be assigned
// USES GLOBAL: none
// MODIFIES GL: ID
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::operator=(const T1 &id) {
    ID = id;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddLft
// DESCRIPTION: Concatenate a left son.
//   ARGUMENTS: Node<T1, T2> *lft - the left son that is to be concatenated
// USES GLOBAL: none
// MODIFIES GL: Lft, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::AddLft(Node<T1, T2> *lft) {

    Lft = lft; // assign the left son

    // update the height
    height = 0;
    if (Lft != NULL)
        height = MAX(height, Lft->height + 1);
    if (Rgt != NULL)
        height = MAX(height, Rgt->height + 1);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddLft
// DESCRIPTION: Concatenate a left son.
//   ARGUMENTS: const T1 &lftID - the ID of the left son that is to be concatenated
//				const T2 * const lftRcd - the record of the left son
// USES GLOBAL: none
// MODIFIES GL: Lft, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::AddLft(const T1 &lftID, const T2 * const lftRcd) {

    Node *Tmp = new Node(lftID, lftRcd);
    Lft = Tmp;

    // update the height
    height = 0;
    if (Lft != NULL)
        height = MAX(height, Lft->height + 1);
    if (Rgt != NULL)
        height = MAX(height, Rgt->height + 1);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddRgt
// DESCRIPTION: Concatenate a right son.
//   ARGUMENTS: Node<T1, T2> *rgt - the right son that is to be concatenated
// USES GLOBAL: none
// MODIFIES GL: Rgt, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::AddRgt(Node<T1, T2> *rgt) {

    Rgt = rgt; // assign the left son

    // update the height
    height = 0;
    if (Lft != NULL)
        height = MAX(height, Lft->height + 1);
    if (Rgt != NULL)
        height = MAX(height, Rgt->height + 1);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AddRgt
// DESCRIPTION: Concatenate a right son.
//   ARGUMENTS: const T1 &rgtID - the ID of the right son that is to be concatenated
//				const T2 * const RgtRcd - the record of the right son
// USES GLOBAL: none
// MODIFIES GL: Rgt, height
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-05
//							KC 2015-02-05
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool Node<T1, T2>::AddRgt(const T1 &rgtID, const T2 * const RgtRcd ) {          // EDIT removed redefinition of default argument

    Node *Tmp = new Node(rgtID, RgtRcd);
    Rgt = Tmp;

    // update the height
    height = 0;
    if (Lft != NULL)
        height = MAX(height, Lft->height + 1);
    if (Rgt != NULL)
        height = MAX(height, Rgt->height + 1);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: print
// DESCRIPTION: To print the Node's ID, height and two sons.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: void
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-11
//							KC 2015-02-11
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
void Node<T1, T2>::print() const{
    cout << ID << ": h-" << height << "  l-";
    if (Lft != NULL)
        cout << '(' << Lft->getID() << ')';
    cout << "  r-";
    if (Rgt != NULL)
        cout << '(' << Rgt->getID() << ')';
    cout << endl;
    if (Lft != NULL)
        Lft->print();
    if (Rgt != NULL)
        Rgt->print();
}

////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////AVL tree/////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2 = NULLT>
class AVLTree {

private :
    Node<T1, T2> *root;
    int size;
    int(*cmp)(const T1 &a, const T1 &b);

    int calcHeight(const Node<T1, T2> * const node) const;
    int calcSize(const Node<T1, T2> * const node) const;
    Node<T1, T2>* rotateLL(Node<T1, T2> *N1);
    Node<T1, T2>* rotateRR(Node<T1, T2> *N1);
    Node<T1, T2>* rotateLR(Node<T1, T2> *N1);
    Node<T1, T2>* rotateRL(Node<T1, T2> *N1);
    Node<T1, T2>* balance(Node<T1, T2> *node);
    Node<T1, T2>* plug(Node<T1, T2> *node, const T1 &id);
    Node<T1, T2>* cut(Node<T1, T2> *node, const T1 &id);
    Node<T1, T2>* findRML(const Node<T1, T2>* const node) const;
public :
    // constructors and destructor
    AVLTree();
    AVLTree(int(*compare)(const T1 &a, const T1 &b));
    AVLTree(const Node<T1, T2> &head, int(*compare)(const T1 &a, const T1 &b) = dCmp);
    AVLTree(const T1 &rootID, const T2 * const rootRcd = NULL, int(*compare)(const T1 &a, const T1 &b) = dCmp);
    AVLTree(const T1 &rootID, const T2 &rootRcd, int(*compare)(const T1 &a, const T1 &b) = dCmp);
    AVLTree(const AVLTree<T1, T2> &New);
    ~AVLTree();

    bool setCmp(int(*compare)(const T1 &a, const T1 &b));
    bool addRoot(const T1 &id, const T2 * const rcd = NULL);
    bool addRoot(const T1 &id, const T2 &rcd);
    bool addRoot(const Node<T1, T2> &New);

    bool Insert(const T1 &id);
    bool Delete(const T1 &id);
    bool empty();

    int getSize() const { return size; }
    int getHeight() const { return root->getHeight(); }
    T2 *find(const T1 &id) const;
    T1 rootID() const { return root->getID(); }
    bool print() const;
};

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Constructor of AVLTree class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree() {
    root = NULL;
    size = 0;
    cmp = dCmp;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Constructor of AVLTree class.
//   ARGUMENTS: int(*compare)(const T1 &a, const T1 &b) - the compare function
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree(int(*compare)(const T1 &a, const T1 &b)) {
    root = NULL;
    size = 0;
    cmp = compare;
}
////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Constructor of AVLTree class.
//   ARGUMENTS: const Node<T1, T2> &head - the root node of the AVL tree
//				int(*compare)(const T1 &a, const T1 &b) = dCmp - the compare function
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree(const Node<T1, T2> &head, int(*compare)(const T1 &a, const T1 &b)) {
    root = new Node<T1, T2>(head.getID(), head.getRcd());
    size = calcSize(root);
    cmp = compare;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Constructor of AVLTree class.
//   ARGUMENTS: const T1 &rootID - the ID of the root node
//				const T2 * const rootRcd = NULL - the initial root record
//				int(*compare)(const T1 &a, const T1 &b) = dCmp - the compare function
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree(const T1 &rootID, const T2 * const rootRcd, int(*compare)(const T1 &a, const T1 &b)) {
    root = new Node<T1, T2>(rootID, rootRcd);
    size = 1;
    cmp = compare;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Constructor of AVLTree class.
//   ARGUMENTS: const T1 &rootID - the ID of the root node
//				const T2 &rootRcd - the initial root record
//				int(*compare)(const T1 &a, const T1 &b) = dCmp - the compare function
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree(const T1 &rootID, const T2 &rootRcd, int(*compare)(const T1 &a, const T1 &b)) {
    root = new Node<T1, T2>(rootID, rootRcd);
    size = 1;
    cmp = compare;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Copy constructor of AVLTree class.
//   ARGUMENTS: const AVLTree<T1, T2> &Old - the AVLTree that is to be copied
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::AVLTree(const AVLTree<T1, T2> &Old) {
    size = Old.size;
    cmp = Old.cmp;
    if (Old.root != NULL) {
        root = new Node<T1, T2>;
        root->copy(Old.root);
    }
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: AVLTree
// DESCRIPTION: Destructor of AVLTree class.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: root, size, cmp;
//     RETURNS: none
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
AVLTree<T1, T2>::~AVLTree() {
    //cout << "Destructor: ";
    //if (root != NULL)
    //	cout << root->getID();
    //cout << endl;
    delete root;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: calcHeight
// DESCRIPTION: To get the height of a node in AVL tree.
//   ARGUMENTS: const Node<T1, T2> * const node - the node of which the height we want
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: int
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
int AVLTree<T1, T2>::calcHeight(const Node<T1, T2> * const node) const {
    if (node == NULL)
        return -1;
    return node->getHeight();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: calcSize
// DESCRIPTION: To calculate the size of the tree with the root "node".
//   ARGUMENTS: const Node<T1, T2> * const node - the root the of tree
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: int
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
int AVLTree<T1, T2>::calcSize(const Node<T1, T2> * const node) const {
    if (node == NULL)
        return 0;
    else
        return calcSize(node->getLft()) + calcSize(node->getRgt()) + 1;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: setCmp
// DESCRIPTION: To assign the compare function to the member function pointer.
//   ARGUMENTS: int(*compare)(const T1 &a, const T1 &b) - the compare function
// USES GLOBAL: none
// MODIFIES GL: cmp
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::setCmp(int(*compare)(const T1 &a, const T1 &b)) {
    cmp = compare;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: addRoot
// DESCRIPTION: To add a root for the AVL tree.
//   ARGUMENTS: const T1 &id - the id of the root node
//				const T2 * const rcd - the record of the root node with default value NULL
// USES GLOBAL: none
// MODIFIES GL: root, size
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::addRoot(const T1 &id, const T2 * const rcd) {
    if (root != NULL) {
        throw AVLERR("root already exists");
    }
    root = new Node<T1, T2>(id, rcd);
    size = calcSize(root);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: addRoot
// DESCRIPTION: To add a root for the AVL tree.
//   ARGUMENTS: const T1 &id - the id of the root node
//				const T2 &rcd - the record of the root node
// USES GLOBAL: none
// MODIFIES GL: root, size
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::addRoot(const T1 &id, const T2 &rcd) {
    if (root != NULL) {
        throw AVLERR("root already exists");
    }
    root = new Node<T1, T2>(id, rcd);
    size = calcSize(root);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: addRoot
// DESCRIPTION: To add a root for the AVL tree.
//   ARGUMENTS: const Node<T1, T2> &New - the copy of the root node
// USES GLOBAL: none
// MODIFIES GL: root, size
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::addRoot(const Node<T1, T2> &New) {
    if (root != NULL) {
        throw AVLERR("root already exists");
    }
    root = new Node<T1, T2>(New);
    size = calcSize(root);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: empty
// DESCRIPTION: To delete all the nodes in the AVL tree.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: root, size
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::empty() {
    if (root == NULL)
        return true;
    delete root;
    root = NULL;
    size = 0;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: find
// DESCRIPTION: To find a node of a certain ID in AVL tree.
//   ARGUMENTS: const T1 &id - the ID of the node that we want to find
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: T2*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
T2 *AVLTree<T1, T2>::find(const T1 &id) const {
    Node<T1, T2> *p = root;
    while ((p != NULL) && (cmp(p->getID(), id) != 0)) {
        if (cmp(id, p->getID()) > 0)
            p = p->getRgt();
        else
            p = p->getLft();
    }
    if (p == NULL)
        return NULL;
    return p->getRcd();
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: rotateLL
// DESCRIPTION: The single rotation LL of AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the trouble finder node
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::rotateLL(Node<T1, T2> *N1) {
    Node<T1, T2> *N2 = N1->getLft();
    N1->AddLft(N2->getRgt());
    N2->AddRgt(N1);
    return N2;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: rotateRR
// DESCRIPTION: The single rotation RR of AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the trouble finder node
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::rotateRR(Node<T1, T2> *N1) {
    Node<T1, T2> *N2 = N1->getRgt();
    N1->AddRgt(N2->getLft());
    N2->AddLft(N1);
    return N2;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: rotateLR
// DESCRIPTION: The single rotation LR of AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the trouble finder node
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::rotateLR(Node<T1, T2> *N1) {
    Node<T1, T2> *N2 = N1->getLft();
    Node<T1, T2> *N3 = N2->getRgt();
    N2->AddRgt(N3->getLft());
    N1->AddLft(N3->getRgt());
    N3->AddLft(N2);
    N3->AddRgt(N1);
    return N3;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: rotateRL
// DESCRIPTION: The single rotation RL of AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the trouble finder node
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::rotateRL(Node<T1, T2> *N1) {
    Node<T1, T2> *N2 = N1->getRgt();
    Node<T1, T2> *N3 = N2->getLft();
    N2->AddLft(N3->getRgt());
    N1->AddRgt(N3->getLft());
    N3->AddLft(N1);
    N3->AddRgt(N2);
    return N3;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: balance
// DESCRIPTION: To balance a subtree whose root is "node".
//   ARGUMENTS: Node<T1, T2> *node - the root of the sub tree
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-12
//							KC 2015-02-12
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::balance(Node<T1, T2> *node) {
    if (node == NULL)
        return node;

    if (calcHeight(node->getLft()) - calcHeight(node->getRgt()) >= 2) {
        if (calcHeight(node->getLft()->getLft()) > calcHeight(node->getLft()->getRgt()))
            node = this->rotateLL(node);
        else
            node = this->rotateLR(node);
    }
    else if (calcHeight(node->getRgt()) - calcHeight(node->getLft()) >= 2) {
        if (calcHeight(node->getRgt()->getRgt()) > calcHeight(node->getRgt()->getLft()))
            node = this->rotateRR(node);
        else
            node = this->rotateRL(node);
    }

    return node;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: plug
// DESCRIPTION: plug a node into the AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the root of the subtree that contains the new node
//				const T1 &id - the id of the new node that is to be plugged in
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::plug(Node<T1, T2> *node, const T1 &id) {
    if (node == NULL) {
        node = new Node<T1, T2>(id);
        ++size;
    }
    else if (cmp(id, node->getID()) > 0) {
        node->AddRgt(this->plug(node->getRgt(), id));
        node = balance(node);
    }
    else if (cmp(id, node->getID()) < 0) {
        node->AddLft(this->plug(node->getLft(), id));
        node = balance(node);
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Insert
// DESCRIPTION: The user interface of inserting a node into the AVL tree.
//   ARGUMENTS: const T1 &id - the id of the new node that is to be inserted
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::Insert(const T1 &id) {
    root = plug(root, id);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: findRML
// DESCRIPTION: To find the rightmost posterity of a node in its left sub-tree.
//   ARGUMENTS: Node<T1, T2>* node - the top node
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-11
//							KC 2015-02-11
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::findRML(const Node<T1, T2>* const node) const{
    Node<T1, T2>* RML = node->getLft();
    if (RML == NULL)
        return NULL;
    while (RML->getRgt() != NULL)
        RML = RML->getRgt();
    return RML;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: cut
// DESCRIPTION: To cut a Node from the AVL tree.
//   ARGUMENTS: Node<T1, T2> *N1 - the root of the subtree that node is deleted from
//				const T1 &id - the id of the new node that is to be deleted
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: Node<T1, T2>*
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-12
//							KC 2015-02-12
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
Node<T1, T2>* AVLTree<T1, T2>::cut(Node<T1, T2> *node, const T1 &id) {
    Node<T1, T2> *tmp;
    if (node == NULL)
        return node;
    if (cmp(id, node->getID()) > 0) {
        tmp = cut(node->getRgt(), id);
        if (tmp == NULL)
            delete node->getRgt();
        node->AddRgt(tmp);
        node = balance(node);
    }
    else if (cmp(id, node->getID()) < 0) {
        tmp = cut(node->getLft(), id);
        if (tmp == NULL)
            delete node->getLft();
        node->AddLft(tmp);
        node = balance(node);
    }
    else {
        int Case;
        Case = 2 * (node->getLft() != NULL) + (node->getRgt() != NULL);

        switch (Case) {
            case 0 :	// node is a leaf
                return NULL;
            case 1 :	// node has no left son
                *node = *(node->getRgt());
                delete node->getRgt();
                node->AddRgt(NULL);
                node = balance(node);
                break;
            default :	// node has a left son, or has both left and right subtree
                Node<T1, T2> *RML = findRML(node);
                *node = *RML;
                tmp = cut(node->getLft(), RML->getID());
                if (tmp == NULL)
                    delete node->getLft();
                node->AddLft(tmp);
                node = balance(node);
                break;
        }
    }
    return node;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: Delete
// DESCRIPTION: The user interface of deleting a node into the AVL tree.
//   ARGUMENTS: const T1 &id - the id of the new node that is to be deleted
// USES GLOBAL: none
// MODIFIES GL: root (possible)
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-10
//							KC 2015-02-10
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::Delete(const T1 &id) {
    root = cut(root, id);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//        NAME: print
// DESCRIPTION: print a tree with inorder traversal.
//   ARGUMENTS: none
// USES GLOBAL: none
// MODIFIES GL: none
//     RETURNS: bool
//      AUTHOR: Kingston Chan
// AUTHOR/DATE: KC 2015-02-11
//							KC 2015-02-11
////////////////////////////////////////////////////////////////////////////////
template<class T1, class T2>
bool AVLTree<T1, T2>::print() const {
    if (root != NULL) {
        root->print();
        return true;
    }
    else
        return false;
}

#endif