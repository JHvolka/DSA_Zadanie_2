#ifndef ZADANIE_2_RB_TREE_H
#define ZADANIE_2_RB_TREE_H

#include <limits>
#include <string>
#include <iostream>
#include <iomanip>

class RB_TREE {
public:
    RB_TREE();

    ~RB_TREE();

    void Insert(size_t key, void *data);

    void *Find(size_t key) const {
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

private:
    enum node_color_t {
        BLACK,
        RED
    };

    struct node_t {
        node_t()
                : parent(nullptr),
                  left(nullptr),
                  right(nullptr),
                  color(RED),
                  data(nullptr) {}

        explicit node_t(size_t key, void *data)
                : parent(nullptr),
                  left(nullptr),
                  right(nullptr),
                  color(RED),
                  key(key),
                  data(data) {}

        node_t *parent;
        node_t *left;
        node_t *right;
        node_color_t color;
        size_t key;
        void *data;
    };

    void rotate_left(node_t *node);

    void rotate_right(node_t *node);

    node_t *m_NIL;
    node_t *m_root;

#if DEBUG == 1
    static void debug_print_node (node_t * node){
        std::cout << std::left << std::setfill('-')
#ifdef linux // Color only on linux
                  << (node->color == RED ? "\033[31m" : "\033[1;34m")
#endif
                  << std::setw(5) << "-Key:" << std::setw(6)
                  << node->key
                  << std::setw(3) << "-p:" << std::setw(6)
                  << reinterpret_cast<void *> (node->parent)
                  << std::setw(3) << "-l:" << std::setw(6)
                  << reinterpret_cast<void *> (node->left)
                  << std::setw(3) << "-r:" << std::setw(6)
                  << reinterpret_cast<void *> (node->right)
                  << std::setw(5) << "-add:" << std::setw(14)
                  << (void *) node
#ifdef linux // Reset color
                  << "\033[0m"
#endif
                  << std::endl;
    }

    void debug_print_rec(node_t *node, bool left, std::string &str,
                     int depth) {
        if (node == m_NIL || depth >= 20) {
            return;
        }
        str.append("    ");
        debug_print_rec(node->left, true, str, ++depth);
        str.pop_back();
        str.pop_back();
        str.pop_back();
        str.pop_back();
        str.pop_back();
#ifdef linux // Pretty symbols only work in linux for some reason?
        std::cout << str << (left ? "╭" : "╰");
#else
        std::cout << str << '+';
#endif
        str.append(" ");
        debug_print_node(node);
        if (left && node->right != m_NIL) {
            str.pop_back();
            str.append("|   |");
        }
        else if (left && node->right == m_NIL)
            str.append("|   ");
        else if (!left && node->right != m_NIL)
            str.append("   |");
        else
            str.append("    ");

        debug_print_rec(node->right, false, str, depth);
        str.pop_back();
        str.pop_back();
        str.pop_back();
        str.pop_back();
        std::cout << std::flush;

    }

public:

    void debug_print() {
        std::string str = " ";
        debug_print_rec(m_root, false, str, 0);
    }
#endif
};

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
        node_t *next;
        if (tmp->left != m_NIL) {
            next = tmp->left;
            tmp->left = m_NIL;
        }
        else if (tmp->right != m_NIL) {
            next = tmp->right;
            tmp->right = m_NIL;
        }
        else {
            next = tmp->parent;
            delete tmp;
        }
        tmp = next;
    }
    delete m_NIL;
}

void RB_TREE::Insert(size_t key, void *data) {
    auto *n_ins = new node_t(key, data);
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


#endif //ZADANIE_2_RB_TREE_H
