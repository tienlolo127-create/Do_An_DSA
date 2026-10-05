#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include <vector>
#include <string>
#include "Product.h"
#include "HashTable.h"
#include "BinaryHeap.h"

class SystemManager {
private:
    std::vector<Product*> masterInventory;

    CustomHashTable<std::string, Product*> barcodeTable;
    CustomHashTable<std::string, Product*> skuTable;
    CustomHashTable<std::string, Invoice> invoiceDB;

    BinaryHeap maxHeapSales;
    BinaryHeap minHeapUnsold;

    std::string invoiceCsvFile;
    std::string productsCsvFile;

public:
    SystemManager();
    ~SystemManager();

    bool initializeData(const std::string& productsFile, const std::string& invoicesFile);

    // [THAO TAC 2 - MC1] TRA CUU CHINH XAC (Custom Hash Table O(1))
    Product* searchByBarcode(const std::string& barcode);
    Product* searchBySKU(const std::string& sku);

    // [THAO TAC 3 - MC2] TRUY XUAT THEO KHOANG GIA & DUY NHAT 1 SP TON KHO THAP NHAT
    std::vector<Product*> getProductsByPriceRange(double minPrice, double maxPrice);
    Product* getSingleLowestStockProduct();

    // [THAO TAC 4 - TP1] TRUY VAN HOA DON & DOI/TRA HANG TRONG 3 NGAY (Custom Hash Table O(1))
    bool queryInvoice(const std::string& invoiceID, bool hasPermission, Invoice& result);
    bool processReturnExchange(const std::string& invoiceID, const std::string& productID, const std::string& requestType, int returnQty);

    // [THAO TAC 5 - TP2] TOP BAN CHAY NHAT (Custom Binary Max-Heap O(1) top)
    void displayTopSelling(int k);

    // [THAO TAC 6 - TP3] HANG DONG VON / CHAM LUAN CHUYEN (Custom Binary Min-Heap O(1) top)
    void displayStagnantInventory(int k);
};

#endif