#include <iostream>
#include "MinHeap.h"

using namespace std;

void swapIndex(int &a, int &b) {
    int temp = a;
    a = b;
    b = temp;
}

void heapify(Product products[], int heap[], int n, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && products[heap[left]].stock < products[heap[smallest]].stock)
        smallest = left;

    if (right < n && products[heap[right]].stock < products[heap[smallest]].stock)
        smallest = right;

    if (smallest != i) {
        swapIndex(heap[i], heap[smallest]);
        heapify(products, heap, n, smallest);
    }
}

void buildMinHeap(Product products[], int heap[], int n) {
    for (int i = 0; i < n; i++) heap[i] = i;

    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(products, heap, n, i);
}

void printMinStock(Product products[], int heap[], int n) {
    if (n <= 0) {
        cout << "Khong co san pham.\n";
        return;
    }

    int pos = heap[0];

    cout << "\nSan pham co ton kho nho nhat:\n";
    cout << products[pos].id << " - "
         << products[pos].name << " - Gia: "
         << products[pos].salePrice << " - Ton kho: "
         << products[pos].stock << endl;
}
