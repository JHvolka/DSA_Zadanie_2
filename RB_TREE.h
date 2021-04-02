#ifndef ZADANIE_2_RB_TREE_H
#define ZADANIE_2_RB_TREE_H

#include <limits>
#include <string>
#include <iostream>
#include <iomanip>

class RB_TREE {
public:
    RB_TREE();

    virtual ~RB_TREE();

    void Insert(int key, void * data);

    void * Find(int key) const;

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

        explicit node_t(int key, void * data)
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
        int key;
        void * data;
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


#endif //ZADANIE_2_RB_TREE_H
