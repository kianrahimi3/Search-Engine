//
// Created by kianr on 4/12/2021.
//

/**
 * This Hash table will be used mainly for getting the authors.
 * Credit can be given to Professor Fontenot for the in class explanation
 */


#ifndef SEARCH_ENGINE_HASH_H
#define SEARCH_ENGINE_HASH_H

#include <iostream>
#include <vector>

#define DEBUG true

using namespace std;

template <typename K, typename V>
class Hash {
private:
    struct Entry {
        K key;
        V value;
        Entry(K k, V v) : key(k), value(v) {}
    };

    vector<Entry>* table = nullptr;
    int num_elements = 0;
    int size = 0;
public:
    Hash() {
        size = 100;
        table = new vector<Entry>[size];
    }

    void insert(const K& theKey, const V& theValue) {
        Entry e(theKey, theValue);
        auto hash_value = hash<K>{}(theKey);
        int location = hash_value % size;
        //     if(DEBUG) cout << theKey << ": "
        //                                     << hash_value << endl;

        table[location].push_back(e);
        num_elements++;

        if(num_elements == size)
            resize();
    }

    void resize() {



    }

    V& get(const K& theKey) {
        auto location = hash<K>{}(theKey) % size;
        for(auto &c : table[location]) {
            if(c.key == theKey)
                return c.value;
        }
    }

    bool find(const K& theKey) {
        auto location = hash<K>{}(theKey) % size;
        for(auto& c : table[location]) {
            if(c.key == theKey)
                return true;
        }
        return false;
    }

    int getSize() {
        return size;
    }
    int getTableSize() {
        return table->size();
    }
    int getCapacity() {
        return num_elements;
    }

    void clear() {
        table->clear();
        num_elements = 0;
    }


};

#endif //SEARCH_ENGINE_HASH_H
