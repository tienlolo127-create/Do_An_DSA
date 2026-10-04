#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include "Product.h"
#include "HashTable.h"

bool loadFromCSV(const std::string& filename, std::vector<Product*>& inventory) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "[LỖI] Không thể mở tệp " << filename << "\n";
        return false;
    }

    std::string line;
    bool isHeader = true;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (isHeader) {
            isHeader = false;
            continue;
        }

        std::stringstream ss(line);
        std::string barcode, sku, name, priceStr, stockStr, shelf;

        if (std::getline(ss, barcode, ',') &&
            std::getline(ss, sku, ',') &&
            std::getline(ss, name, ',') &&
            std::getline(ss, priceStr, ',') &&
            std::getline(ss, stockStr, ',') &&
            std::getline(ss, shelf, ',')) {

            double price = std::stod(priceStr);
            int stock = std::stoi(stockStr);

            inventory.push_back(new Product(barcode, sku, name, price, stock, shelf));
        }
    }
    file.close();
    return true;
}

int main() {
    std::vector<Product*> inventory;
    std::string csvPath = "products.csv";

    if (!loadFromCSV(csvPath, inventory)) {
        std::cout << "Không thể khởi tạo dữ liệu từ tệp CSV. Chương trình kết thúc.\n";
        return 1;
    }

    ProductHashTable barcodeTable;
    ProductHashTable skuTable;

    for (Product* p : inventory) {
        barcodeTable.insert(p->barcode, p);
        skuTable.insert(p->sku, p);
    }

    std::cout << "===================================================\n";
    std::cout << " HE THONG QUAN LY BAN LE TAP HOA - MODULE MC1      \n";
    std::cout << " (Tra cuu chinh xac quy mo lon theo Barcode/SKU)   \n";
    std::cout << "===================================================\n";
    std::cout << " [Thong ke] Da nap " << inventory.size() << " mat hang tu tep CSV vao Index.\n\n";

    std::string searchInput;
    while (true) {
        std::cout << "Nhap Ma vach (Barcode) hoac Ma SKU (Nhap 'exit' de thoat): ";
        std::cin >> searchInput;

        if (searchInput == "exit" || searchInput == "EXIT") {
            break;
        }

        Product* foundProduct = barcodeTable.search(searchInput);

        if (!foundProduct) {
            foundProduct = skuTable.search(searchInput);
        }

        printProductDetails(foundProduct);
    }

    for (Product* p : inventory) {
        delete p;
    }

    std::cout << "Da thoat chuong trinh tra cuu MC1 thanh cong.\n";
    return 0;
}