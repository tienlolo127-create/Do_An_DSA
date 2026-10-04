#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include "Product.h"

using namespace std;


class Maxheap {
private:
    vector<Item> heap;          
    map<int, int> pos; 
 
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }
    
    
    bool isBetter(const Item& a, const Item& b) const {
        if (a.total_count != b.total_count) {
            return a.total_count > b.total_count; 
        }
        return a.id < b.id;  
    }
    void swap_items(int i, int j){
        swap(heap[i], heap[j]);
        pos[heap[i].id] = i;
        pos[heap[j].id] = j;
    }

    void sift_up(int i) {
        while (i > 0) {
            int p = parent(i);
            if (isBetter(heap[i], heap[p])) {  
                swap_items(i, p);
                i = p;
            } else {
                break;
            }
        }
    }
    
    void sift_down(int i) {
        int n = heap.size();
        while (true) {
            int largest = i;
            int l = left(i);
            int r = right(i);
            
            if (l < n && isBetter(heap[l], heap[largest])) {  
                largest = l;
            }
            if (r < n && isBetter(heap[r], heap[largest])) {  
                largest = r;
            }
            
            if (largest != i) {
                swap_items(i, largest);
                i = largest;
            } else {
                break;
            }
        }
    }
    
public:
    Maxheap(const vector<Item>& items) {
        heap = items;
        int n = heap.size();
        
        for (int i = 0; i < n; i++) {
            pos[heap[i].id] = i;
        }
        for (int i = n / 2 - 1; i >= 0; i--) {
            sift_down(i);
        }
    }
    
    void update(int id, long long count) {
        if (pos.find(id) == pos.end()) return;
        
        int index = pos[id];
        heap[index].total_count += count;
        
        sift_up(index);
       
    }
    
    vector<Item> getTopK(int k) {
        int n = heap.size();
        k = min(k, n);
    
    vector<Item> result;
    
    
    vector<Item> temp = heap;
    

    for (int i = 0; i < k; i++) {
        result.push_back(temp[0]);  
        
      
        temp[0] = temp.back();
        temp.pop_back();
        
      
        int p = 0;
        int d = temp.size();
        while (true) {
            int largest = p;
            int l = 2 * p + 1;
            int r = 2 * p + 2;
            
            if (l < d && (isBetter(temp[l], temp[largest]))) {
                largest = l;
            }
            if (r < d && (isBetter(temp[r], temp[largest]))) {
                largest = r;
            }
            
            if (largest != p) {
                swap(temp[p], temp[largest]);
                p = largest;
            } else {
                break;
            }
        }
    }
    
    return result;
    }

};

#endif