#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Product.h"
#include <string>

struct HashNode {
    std::string key;
    Product* productPtr;
    HashNode* next;

    HashNode(std::string k, Product* p) : key(k), productPtr(p), next(nullptr) {}
};

class ProductHashTable {
private:
    HashNode** table;
    size_t capacity;
    size_t count;
    float maxLoadFactor;

    size_t hashFunction(const std::string& key) const;
    void rehash();

public:
    ProductHashTable(size_t initialCapacity = 10007, float loadFactor = 0.75f);
    ~ProductHashTable();

    bool insert(const std::string& key, Product* product);
    Product* search(const std::string& key) const;
    bool remove(const std::string& key);

    size_t getCount() const { return count; }
    size_t getCapacity() const { return capacity; }
    float getCurrentLoadFactor() const { return static_cast<float>(count) / capacity; }
};

#endif