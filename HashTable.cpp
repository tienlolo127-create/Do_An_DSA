#include "HashTable.h"

ProductHashTable::ProductHashTable(size_t initialCapacity, float loadFactor)
    : capacity(initialCapacity), count(0), maxLoadFactor(loadFactor) {
    table = new HashNode*[capacity]();
}

ProductHashTable::~ProductHashTable() {
    for (size_t i = 0; i < capacity; ++i) {
        HashNode* current = table[i];
        while (current) {
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] table;
}

size_t ProductHashTable::hashFunction(const std::string& key) const {
    size_t hash = 2166136261U;
    for (char c : key) {
        hash ^= static_cast<unsigned char>(c);
        hash *= 16777619U;
    }
    return hash % capacity;
}

void ProductHashTable::rehash() {
    size_t oldCapacity = capacity;
    HashNode** oldTable = table;

    capacity = capacity * 2 + 1;
    table = new HashNode*[capacity]();
    count = 0;

    for (size_t i = 0; i < oldCapacity; ++i) {
        HashNode* current = oldTable[i];
        while (current) {
            insert(current->key, current->productPtr);
            HashNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] oldTable;
}

bool ProductHashTable::insert(const std::string& key, Product* product) {
    if (key.empty() || !product) return false;

    if (getCurrentLoadFactor() > maxLoadFactor) {
        rehash();
    }

    size_t index = hashFunction(key);
    HashNode* current = table[index];

    while (current) {
        if (current->key == key) {
            current->productPtr = product;
            return true;
        }
        current = current->next;
    }

    HashNode* newNode = new HashNode(key, product);
    newNode->next = table[index];
    table[index] = newNode;
    count++;
    return true;
}

Product* ProductHashTable::search(const std::string& key) const {
    if (key.empty()) return nullptr;

    size_t index = hashFunction(key);
    HashNode* current = table[index];

    while (current) {
        if (current->key == key) {
            return current->productPtr;
        }
        current = current->next;
    }
    return nullptr;
}

bool ProductHashTable::remove(const std::string& key) {
    if (key.empty()) return false;

    size_t index = hashFunction(key);
    HashNode* current = table[index];
    HashNode* prev = nullptr;

    while (current) {
        if (current->key == key) {
            if (prev) {
                prev->next = current->next;
            } else {
                table[index] = current->next;
            }
            delete current;
            count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}