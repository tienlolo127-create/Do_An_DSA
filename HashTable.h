#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <string>
#include <vector>
#include <iostream>
#include "Debug.h"

template<typename K, typename V>
class CustomHashTable {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    HashNode** table;
    size_t capacity;
    size_t count;
    float maxLoadFactor;

    size_t hashFunction(const std::string& key) const {
        size_t hash = 0;
        for (int i = 0; i < key.length(); i++) {
            hash = hash * 31 + key[i];
        }
        return hash % capacity;
    }

    size_t hashFunction(long long key) const {
        if (key < 0) key = -key;
        return key % capacity;
    }

    void rehash() {
        size_t oldCapacity = capacity;
        HashNode** oldTable = table;

        capacity = oldCapacity * 2 + 1;
        table = new HashNode*[capacity]();
        count = 0;

        LOG_DEBUG("REHASH HASH TABLE: Dung luong tu " << oldCapacity << " -> " << capacity);

        for (size_t i = 0; i < oldCapacity; ++i) {
            HashNode* curr = oldTable[i];
            while (curr) {
                HashNode* nextNode = curr->next;
                insert(curr->key, curr->value);
                delete curr;
                curr = nextNode;
            }
        }
        delete[] oldTable;
    }

public:
    CustomHashTable(size_t initCap = 101, float loadFactor = 0.75f)
        : capacity(initCap), count(0), maxLoadFactor(loadFactor) {
        table = new HashNode*[capacity]();
    }

    ~CustomHashTable() {
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* curr = table[i];
            while (curr) {
                HashNode* tmp = curr;
                curr = curr->next;
                delete tmp;
            }
        }
        delete[] table;
    }

    void insert(const K& key, const V& value) {
        if (static_cast<float>(count + 1) / capacity > maxLoadFactor) {
            rehash();
        }

        size_t index = hashFunction(key);
        HashNode* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return;
            }
            curr = curr->next;
        }

        HashNode* newNode = new HashNode(key, value);
        newNode->next = table[index];
        table[index] = newNode;
        count++;

        LOG_DEBUG("CustomHashTable Insert: Key=" << key << " -> Bucket Index=" << index);
    }

    bool get(const K& key, V& out_value) const {
        size_t index = hashFunction(key);
        HashNode* curr = table[index];

        while (curr) {
            if (curr->key == key) {
                out_value = curr->value;
                LOG_DEBUG("CustomHashTable Search MATCH: Key=" << key);
                return true;
            }
            curr = curr->next;
        }
        LOG_DEBUG("CustomHashTable Search NOT FOUND: Key=" << key);
        return false;
    }

    std::vector<V> getAllValues() const {
        std::vector<V> values;
        for (size_t i = 0; i < capacity; ++i) {
            HashNode* curr = table[i];
            while (curr) {
                values.push_back(curr->value);
                curr = curr->next;
            }
        }
        return values;
    }

    size_t getSize() const { return count; }
};

#endif