#include "DataLoader.h"
#include "Debug.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

static std::string trimString(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

static std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, ',')) {
        tokens.push_back(trimString(token));
    }
    return tokens;
}

bool loadMasterCSV(const std::string& filename, std::vector<Product*>& inventory) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    bool isHeader = true;
    int countLoaded = 0;

    while (std::getline(file, line)) {
        line = trimString(line);
        if (line.empty()) continue;

        if (isHeader) {
            if (line.size() >= 3 && (unsigned char)line[0] == 0xEF && (unsigned char)line[1] == 0xBB && (unsigned char)line[2] == 0xBF) {
                line = line.substr(3);
            }
            isHeader = false;
            continue;
        }

        std::vector<std::string> tokens = splitCSVLine(line);
        if (tokens.size() >= 8) {
            std::string barcode = tokens[0];
            std::string sku = tokens[1];
            std::string name = tokens[2];
            std::string priceStr = tokens[3];
            std::string stockStr = tokens[4];
            std::string shelf = tokens[5];
            std::string soldStr = tokens[6];
            std::string lastSoldDate = tokens[7];

            if (!barcode.empty() && !sku.empty()) {
                try {
                    double price = std::stod(priceStr);
                    int stock = std::stoi(stockStr);
                    int sold = std::stoi(soldStr);
                    inventory.push_back(new Product(barcode, sku, name, price, stock, shelf, sold, lastSoldDate));
                    countLoaded++;
                } catch (...) {}
            }
        }
    }
    file.close();
    LOG_DEBUG("LOAD PRODUCTS CSV: Da nap thanh cong " << countLoaded << " san pham.");
    return countLoaded > 0;
}
bool saveProductsCSV(const std::string& filename, const std::vector<Product*>& inventory) {
    std::ofstream file(filename, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "Loi: Khong the mo file de ghi: " << filename << std::endl;
        return false;
    }

    file << "Barcode,SKU,TenSP,GiaBan,TonKho,ViTriKe,DaBanTichLuy,NgayBanGanNhat\n";
    for (const Product* p : inventory) {
        if (!p) continue;
        file << p->barcode << ","
             << p->productID << ","
             << p->name << ","
             << std::fixed << std::setprecision(0) << p->price << ","
             << p->stockQuantity << ","
             << p->shelfLocation << ","
             << p->totalSoldQuantity << ","
             << p->lastSoldDate << "\n";
    }
    file.close();
    LOG_DEBUG("SAVE PRODUCTS CSV: Da luu thanh cong " << inventory.size() << " san pham vao file.");
    return true;
}

bool loadInvoicesCSV(const std::string& filename, CustomHashTable<std::string, Invoice>& invoiceDB) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    std::string line;
    Invoice currentInv;
    bool hasInvoice = false;
    int countLoaded = 0;

    while (std::getline(file, line)) {
        line = trimString(line);
        if (line.empty()) continue;

        std::vector<std::string> tokens = splitCSVLine(line);
        if (tokens.empty()) continue;

        std::string type = tokens[0];

        if (type == "INVOICE" && tokens.size() >= 5) {
            if (hasInvoice) {
                invoiceDB.insert(currentInv.invoiceID, currentInv);
                countLoaded++;
            }
            currentInv = Invoice();
            currentInv.invoiceID = tokens[1];
            currentInv.purchase_time = static_cast<time_t>(std::stoll(tokens[2]));
            currentInv.total_paid = std::stod(tokens[3]);
            currentInv.status = tokens[4];
            hasInvoice = true;
        }
        else if (type == "PRODUCT" && hasInvoice && tokens.size() >= 7) {
            Product p;
            p.productID = tokens[1];
            p.name = tokens[2];
            p.invoiceQuantity = std::stoi(tokens[3]);
            p.returnedQuantity = std::stoi(tokens[4]);
            p.price = std::stod(tokens[5]);
            p.discountApplied = std::stod(tokens[6]);
            currentInv.products.push_back(p);
        }
    }
    if (hasInvoice) {
        invoiceDB.insert(currentInv.invoiceID, currentInv);
        countLoaded++;
    }

    file.close();
    LOG_DEBUG("LOAD INVOICES CSV: Da nap " << countLoaded << " hoa don.");
    return countLoaded > 0;
}

bool saveInvoicesCSV(const std::string& filename, const CustomHashTable<std::string, Invoice>& invoiceDB) {
    std::ofstream file(filename);
    if (!file.is_open()) return false;

    std::vector<Invoice> allInvoices = invoiceDB.getAllValues();
    for (const auto& inv : allInvoices) {
        file << "INVOICE," << inv.invoiceID << "," << inv.purchase_time << "," 
             << inv.total_paid << "," << inv.status << "\n";
        for (const auto& p : inv.products) {
            file << "PRODUCT," << p.productID << "," << p.name << "," 
                 << p.invoiceQuantity << "," << p.returnedQuantity << "," 
                 << p.price << "," << p.discountApplied << "\n";
        }
        file << "\n";
    }
    file.close();
    LOG_DEBUG("SAVE INVOICES CSV: Da luu thanh cong " << allInvoices.size() << " hoa don vao file.");
    return true;
}