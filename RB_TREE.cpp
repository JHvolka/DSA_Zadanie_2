//
// Created by Jakub Hvolka on 19/03/2021.
//

#include "RB_TREE.h"


RB_TREE::RB_TREE()
        : m_NIL(new node_t),
          m_root(m_NIL) {
    // Initialize sentinel node
    m_NIL->left = m_NIL->right = m_NIL->parent = m_NIL;
    m_NIL->color = BLACK;
}

RB_TREE::~RB_TREE() {
    node_t *tmp = m_root;
    while (tmp != m_NIL) {
        node_t * next;
        if (tmp->left != m_NIL) {
            next = tmp->left;
            tmp->left = m_NIL;
        }
        else if (tmp->right != m_NIL){
            next = tmp->right;
            tmp->right = m_NIL;
        }
        else{
            next = tmp->parent;
            delete tmp;
        }
        tmp = next;
    }
    delete m_NIL;
}

void RB_TREE::Insert(int key, void * data) {
    auto *n_ins = new node_t(key,data);
    node_t *tmp_x = m_root;
    node_t *tmp_y = m_NIL;

    n_ins->parent = m_NIL;
    n_ins->left = m_NIL;
    n_ins->right = m_NIL;


    // Find position where n_ins is to be inserted
    while (tmp_x != m_NIL) {
        tmp_y = tmp_x;
        if (n_ins->key <= tmp_x->key)
            tmp_x = tmp_x->left;
        else
            tmp_x = tmp_x->right;
    }

    // Perform insertion
    n_ins->parent = tmp_y;
    if (tmp_y == m_NIL)
        m_root = n_ins;
    else if (n_ins->key <= tmp_y->key)
        tmp_y->left = n_ins;
    else
        tmp_y->right = n_ins;

    // Each new node is red
    n_ins->color = RED;

    // Rebalance tree
    while (n_ins != m_root &&
           n_ins->parent->color == RED) {
        if (n_ins->parent == n_ins->parent->parent->left) {
            tmp_y = n_ins->parent->parent->right;
            if (tmp_y->color == RED) {
                n_ins->parent->color = BLACK;
                tmp_y->color = BLACK;
                n_ins->parent->parent->color = RED;
                n_ins = n_ins->parent->parent;
            }
            else {
                if (n_ins == n_ins->parent->right) {
                    n_ins = n_ins->parent;
                    rotate_left(n_ins);
                }
                n_ins->parent->color = BLACK;
                n_ins->parent->parent->color = RED;
                rotate_right(n_ins->parent->parent);
            }
        }


        else {
            tmp_y = n_ins->parent->parent->left;
            if (tmp_y->color == RED) {
                n_ins->parent->color = BLACK;
                tmp_y->color = BLACK;
                n_ins->parent->parent->color = RED;
                n_ins = n_ins->parent->parent;
            }
            else {
                if (n_ins == n_ins->parent->left) {
                    n_ins = n_ins->parent;
                    rotate_right(n_ins);
                }
                n_ins->parent->color = BLACK;
                n_ins->parent->parent->color = RED;
                rotate_left(n_ins->parent->parent);
            }
        }
    }
    // Trivial case, if m_root it red, set it to black.
    // This is performed unconditionally, as m_root never has a red parent
    m_root->color = BLACK;
}

void * RB_TREE::Find(int key) const {
    node_t *tmp = m_root;
    while (tmp != m_NIL) {
        if (tmp->key < key)
            tmp = tmp->left;
        else if (tmp->key > key)
            tmp = tmp->right;
        else if (tmp->key == key)
            return tmp->data;
        else
            break;
    }
    return nullptr;
}

void RB_TREE::rotate_left(RB_TREE::node_t *node) {
    node_t *right = node->right;
    node->right = right->left;

    if (right->left != m_NIL)
        right->left->parent = node;

    right->parent = node->parent;

    if (node->parent == m_NIL)
        m_root = right;
    else if (node == node->parent->left)
        node->parent->left = right;
    else
        node->parent->right = right;

    right->left = node;
    node->parent = right;
}

void RB_TREE::rotate_right(RB_TREE::node_t *node) {
    node_t *left = node->left;
    node->left = left->right;

    if (left->right != m_NIL)
        left->right->parent = node;

    left->parent = node->parent;

    if (node->parent == m_NIL)
        m_root = left;
    else if (node == node->parent->right)
        node->parent->right = left;
    else
        node->parent->left = left;

    left->right = node;
    node->parent = left;
}
