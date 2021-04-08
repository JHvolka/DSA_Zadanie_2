#ifndef ZADANIE_2_HASHROBIN_H
#define ZADANIE_2_HASHROBIN_H


#include <array>
#include <climits>
#include <random>
#include <iostream>
#include <iomanip>

template<typename K, typename V = K>
class HashRobin {
    static const size_t INITIAL_SIZE = 128;
    static const int LOAD_FACTOR = 90;
    static const size_t MERSENE_PRIME = (1LL << 31) - 1;
    size_t seed[5]{};

public:

    HashRobin();

    virtual ~HashRobin() {
        delete[] table;
    }

    V Find(K key);

    void Insert(K key, V val);

private:

    void Insert_internal(size_t hash, K &key, V &val);

    size_t Hash(K key) const;

    void Resize();;

    size_t Distance(size_t pos) const;

    struct Node {
        K key;
        V val;
        size_t hash;

        Node() : key(K()), val(V()), hash(0) {};

        Node(K key, V val, size_t hash) : key(key), val(val), hash(hash) {};

        inline void Swap(K &o_key, V &o_val, size_t &o_hash);;
    };

    Node *table;
    size_t size;
    size_t filled;
    size_t resize_threshold;
    size_t mask;

};

template<typename K, typename V>
void HashRobin<K, V>::Node::Swap(K &o_key, V &o_val, size_t &o_hash) {
    std::swap(o_key, key);
    std::swap(o_val, val);
    std::swap(o_hash, hash);
}

template<typename K, typename V>
HashRobin<K, V>::HashRobin()
        : table(nullptr), size(INITIAL_SIZE), filled(0),
          resize_threshold((INITIAL_SIZE * LOAD_FACTOR) / 100),
          mask(size - 1),
          seed(){
    table = new typename HashRobin<K, V>::Node[INITIAL_SIZE];
    for (int i = 0; i < INITIAL_SIZE; ++i) {
        // SIZE_MAX is used as "unused" value
        table[i].hash = 0;
    }

    std::mt19937_64 rd(12345);
    std::uniform_int_distribution<size_t> dist(0, SIZE_MAX / 2);
    seed[0] = dist(rd);
    seed[1] = dist(rd);
    seed[2] = dist(rd);
    seed[3] = dist(rd);
    seed[4] = dist(rd);
}

template<typename K, typename V>
V HashRobin<K, V>::Find(K key) {
    size_t hash = Hash(key);
    if (hash == 0)
        return V();

    size_t pos = (Hash(key) - 1) & mask;
    while (table[pos].key != key)
        pos = (++pos) & mask;
    return table[pos].val;
}

template<typename K, typename V>
void HashRobin<K, V>::Insert(K key, V val) {
    size_t hash = Hash(key);

    // Resize table if threshold is reached
    if (++filled >= resize_threshold)
        Resize();

    Insert_internal(hash, key, val);
}

template<typename K, typename V>
void HashRobin<K, V>::Insert_internal(size_t hash, K &key, V &val) {
    size_t pos = hash & mask;
    size_t distance = 0;

    while (table[pos].hash != 0) {
        if (Distance(pos) < distance) {
            table[pos].Swap(key, val, hash);
            distance = Distance(pos);
        }
        pos = (pos + 1) & mask;
        ++distance;
    }
    new(&table[pos]) Node(std::move(key), std::move(val), hash);
}

template<typename K, typename V>
size_t HashRobin<K, V>::Hash(K key) const {
    size_t result = 0;
    for (auto i : seed) {
        result = (i + key * result) % MERSENE_PRIME;
    }
    // Add one so that (hash == 0) can be used for "unused value"
    return result + 1;
}

template<typename K, typename V>
void HashRobin<K, V>::Resize() {
    auto old_table = table;
    auto old_size = size;

    size *= 2;

    table = new typename HashRobin<K, V>::Node[size];
    resize_threshold = size * LOAD_FACTOR / 100;
    mask = (size - 1);


    for (int i = 0; i < size; ++i)
        table[i].hash = 0;

    for (size_t i = 0; i < old_size; ++i) {
        if (old_table[i].hash != 0)
            Insert_internal(old_table[i].hash, old_table[i].key,
                            old_table[i].val);
    }
    delete[] old_table;
}

template<typename K, typename V>
size_t HashRobin<K, V>::Distance(size_t pos) const {
    return table[pos].hash & mask;
}


#endif //ZADANIE_2_HASHROBIN_H
