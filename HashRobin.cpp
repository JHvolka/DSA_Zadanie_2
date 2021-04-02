#include "HashRobin.h"







/*
template<typename K, typename V>
HashRobin<K, V>::HashRobin()
        : table(new Node[INITIAL_SIZE]),
          size(INITIAL_SIZE),
          filled(0),
          resize_threshold() {
    for (size_t i = 0; i < INITIAL_SIZE; ++i) {
        table[i].hash = 0;
    }
}

template<typename K, typename V>
V HashRobin<K, V>::Find(K key) {
    return nullptr;
}

template<typename K, typename V>
void HashRobin<K, V>::Insert(K key, V val) {
    if (++filled >= (resize_threshold * size / 100)) {
        // TODO resize
    }
    size_t pos = Find_slot(key);
    new(&table[pos]) Node(std::move(key), std::move(val));
}

template<typename K, typename V>
size_t HashRobin<K, V>::Find_slot(K key) {
    size_t pos = Hash(key) % size;
    size_t dist = 0;
    while (true) {
        if (table[pos].hash == 0)
            return pos;

    }

    while (table[pos].key != K() && table[pos].key != key)
        pos = (pos + 1) % size;
    return pos;
}

template<typename K, typename V>
size_t HashRobin<K, V>::Hash(K key) {
    */
/* Given the assignment, I don't intend to use hash function for int
     * provided in standard library.
     * If not for that, just another template
     * parameter (typename Hash = std::hash<K>) would be sufficient to provide
     * Hash functions for all basic types and more.
     *
     * Only hash function I will implement is for integers (I will test only
     * integers). Though I will give here the option to use other basic types as
     * keys in else statement.
     *//*

    if constexpr (typeid(K) == typeid(int)) {
        // TODO
    }

    else
        return std::hash<K>(key);
}
*/
