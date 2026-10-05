#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <iostream>

struct Product {
    std::string barcode;
    std::string sku;
    std::string name;
    double price;
    int stockQuantity;
    std::string shelfLocation;

    Product(std::string b, std::string s, std::string n, double p, int q, std::string loc)
        : barcode(b), sku(s), name(n), price(p), stockQuantity(q), shelfLocation(loc) {}
};

void printProductDetails(const Product* p);

#endif