#ifndef BINARYHEAP_H
#define BINARYHEAP_H

#include <vector>
#include <algorithm>
#include "Product.h"
#include "Debug.h"

class BinaryHeap {
private:
    std::vector<Product*> heap;
    bool isMaxHeap; // true: Max-Heap (TP2), false: Min-Heap (TP3)

    bool compare(Product* a, Product* b) const {
        if (!a || !b) return false;
        if (isMaxHeap) {
            return a->totalSoldQuantity > b->totalSoldQuantity; // TP2
        } else {
            return a->lastSoldDate < b->lastSoldDate;           // TP3
        }
    }

    void updateIndex(int idx) {
        if (idx < 0 || idx >= static_cast<int>(heap.size())) return;
        if (isMaxHeap) heap[idx]->maxHeapIndex = idx;
        else heap[idx]->minHeapIndex = idx;
    }

    void heapifyUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (compare(heap[idx], heap[parent])) {
                std::swap(heap[idx], heap[parent]);
                updateIndex(idx);
                updateIndex(parent);
                idx = parent;
            } else {
                break;
            }
        }
    }

    void heapifyDown(int idx) {
        int n = static_cast<int>(heap.size());
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int target = idx;

            if (left < n && compare(heap[left], heap[target])) target = left;
            if (right < n && compare(heap[right], heap[target])) target = right;

            if (target != idx) {
                std::swap(heap[idx], heap[target]);
                updateIndex(idx);
                updateIndex(target);
                idx = target;
            } else {
                break;
            }
        }
    }

public:
    BinaryHeap(bool maxHeap = true) : isMaxHeap(maxHeap) {}

    void push(Product* p) {
        if (!p) return;
        heap.push_back(p);
        int idx = static_cast<int>(heap.size()) - 1;
        updateIndex(idx);
        heapifyUp(idx);
    }

    Product* top() const {
        return heap.empty() ? nullptr : heap[0];
    }

    void pop() {
        if (heap.empty()) return;
        int n = static_cast<int>(heap.size());
        if (isMaxHeap) heap[0]->maxHeapIndex = -1;
        else heap[0]->minHeapIndex = -1;

        heap[0] = heap[n - 1];
        heap.pop_back();

        if (!heap.empty()) {
            updateIndex(0);
            heapifyDown(0);
        }
    }

    void updateItem(Product* p) {
        if (!p) return;
        int idx = isMaxHeap ? p->maxHeapIndex : p->minHeapIndex;
        if (idx < 0 || idx >= static_cast<int>(heap.size())) {
            push(p);
            return;
        }
        heapifyUp(idx);
        heapifyDown(idx);
    }

    bool empty() const { return heap.empty(); }
    size_t size() const { return heap.size(); }
};

#endif