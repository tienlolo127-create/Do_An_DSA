#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <ctime>

struct Product {
    std::string productID;        // SKU / ProductID (VD: P001)
    std::string barcode;          // EAN-13
    std::string name;             // Ten san pham
    double price;                 // Gia ban (VND)
    int stockQuantity;            // So luong ton kho
    std::string shelfLocation;    // Vi tri ke
    int totalSoldQuantity;        // Tong so luong da ban
    std::string lastSoldDate;     // Ngay ban gan nhat YYYY-MM-DD

    int invoiceQuantity;          // So luong mua trong hoa don
    int returnedQuantity;         // So luong da doi/tra trong hoa don
    double discountApplied;       // Muc giam gia da ap dung (VND)

    int maxHeapIndex;
    int minHeapIndex;

    Product() 
        : productID(""), barcode(""), name(""), price(0.0), stockQuantity(0), 
          shelfLocation(""), totalSoldQuantity(0), lastSoldDate(""),
          invoiceQuantity(0), returnedQuantity(0), discountApplied(0.0),
          maxHeapIndex(-1), minHeapIndex(-1) {}

    Product(std::string b, std::string s, std::string n, double p, int q, std::string loc, int sold, std::string date)
        : productID(s), barcode(b), name(n), price(p), stockQuantity(q), shelfLocation(loc),
          totalSoldQuantity(sold), lastSoldDate(date), invoiceQuantity(0), returnedQuantity(0),
          discountApplied(0.0), maxHeapIndex(-1), minHeapIndex(-1) {}
};

struct Invoice {
    std::string invoiceID;
    time_t purchase_time;
    std::vector<Product> products;
    double total_paid;
    std::string status;

    Invoice() : invoiceID(""), purchase_time(0), total_paid(0.0), status("Completed") {}
};

inline void printProductDetails(const Product* p) {
    if (!p) return;
    std::cout << "--------------------------------------------------------\n";
    std::cout << " Ma vach (Barcode) : " << p->barcode << "\n";
    std::cout << " Ma SKU / ProductID: " << p->productID << "\n";
    std::cout << " Ten san pham      : " << p->name << "\n";
    std::cout << " Gia ban           : " << std::fixed << std::setprecision(0) << p->price << " VND\n";
    std::cout << " Ton kho           : " << p->stockQuantity << "\n";
    std::cout << " Vi tri ke         : " << p->shelfLocation << "\n";
    std::cout << " Da ban tich luy   : " << p->totalSoldQuantity << "\n";
    std::cout << " Ngay ban gan nhat : " << p->lastSoldDate << "\n";
    std::cout << "--------------------------------------------------------\n";
}

#endif