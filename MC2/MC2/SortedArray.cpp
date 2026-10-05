#include <iostream>
#include "SortedArray.h"

using namespace std;

void merge(Product products[], int index[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int* L = new int[n1];
    int* R = new int[n2];

    for (int i = 0; i < n1; i++) L[i] = index[left + i];
    for (int i = 0; i < n2; i++) R[i] = index[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (products[L[i]].salePrice <= products[R[j]].salePrice)
            index[k++] = L[i++];
        else
            index[k++] = R[j++];
    }

    while (i < n1) index[k++] = L[i++];
    while (j < n2) index[k++] = R[j++];

    delete[] L;
    delete[] R;
}

void mergeSort(Product products[], int index[], int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(products, index, left, mid);
    mergeSort(products, index, mid + 1, right);
    merge(products, index, left, mid, right);
}

void buildSortedArray(Product products[], int index[], int n) {
    for (int i = 0; i < n; i++) index[i] = i;
    if (n > 1) mergeSort(products, index, 0, n - 1);
}

int lowerBound(Product products[], int index[], int n, int minPrice) {
    int left = 0, right = n;

    while (left < right) {
        int mid = left + (right - left) / 2;

        if (products[index[mid]].salePrice < minPrice)
            left = mid + 1;
        else
            right = mid;
    }

    return left;
}

void rangeQuery(Product products[], int index[], int n, int minPrice, int maxPrice) {
    if (minPrice > maxPrice) {
        int temp = minPrice;
        minPrice = maxPrice;
        maxPrice = temp;
    }

    int start = lowerBound(products, index, n, minPrice);
    bool found = false;

    cout << "\nSan pham co gia tu " << minPrice << " den " << maxPrice << ":\n";

    for (int i = start; i < n; i++) {
        int pos = index[i];

        if (products[pos].salePrice > maxPrice) break;

        cout << products[pos].id << " - "
             << products[pos].name << " - Gia: "
             << products[pos].salePrice << " - Ton kho: "
             << products[pos].stock << endl;

        found = true;
    }

    if (!found) cout << "Khong co san pham nao trong khoang gia nay.\n";
}
