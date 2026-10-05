#include <iostream>
#include "CSVReader.h"
#include "SortedArray.h"
#include "MinHeap.h"

using namespace std;

int main() {
    const int MAX_SIZE = 100000;

    // Cap phat dong de tranh tran stack khi benchmark 100000 san pham.
    Product* products = new Product[MAX_SIZE];

    cout << "Dang doc file product.csv...\n";
    int n = readCSV("product.csv", products, MAX_SIZE);

    if (n == 0) {
        cout << "Khong co du lieu.\n";
        delete[] products;
        return 0;
    }

    cout << "Da doc " << n << " san pham.\n";

    int choice;

    cout << "\n===== MENU MC2 =====\n";
    cout << "1. Tim san pham theo khoang gia\n";
    cout << "2. Tim san pham co ton kho nho nhat\n";
    cout << "0. Thoat\n";
    cout << "Lua chon: ";
    cin >> choice;

    if (choice == 1) {
        int* sortedIndex = new int[n];
        buildSortedArray(products, sortedIndex, n);

        int minPrice, maxPrice;
        cout << "Nhap gia thap nhat: ";
        cin >> minPrice;
        cout << "Nhap gia cao nhat: ";
        cin >> maxPrice;

        rangeQuery(products, sortedIndex, n, minPrice, maxPrice);
        delete[] sortedIndex;
    }
    else if (choice == 2) {
        int* heap = new int[n];
        buildMinHeap(products, heap, n);
        printMinStock(products, heap, n);
        delete[] heap;
    }
    else if (choice == 0) {
        cout << "Ket thuc chuong trinh.\n";
    }
    else {
        cout << "Lua chon khong hop le.\n";
    }

    delete[] products;

    cout << "\nNhan Enter de ket thuc...";
    cin.ignore();
    cin.get();

    return 0;
}
