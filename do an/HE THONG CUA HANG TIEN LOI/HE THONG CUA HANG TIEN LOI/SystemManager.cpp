#include "SystemManager.h"
#include "DataLoader.h"
#include "Debug.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <ctime>

SystemManager::SystemManager() 
    : barcodeTable(101, 0.75f), skuTable(101, 0.75f), invoiceDB(101, 0.75f),
      maxHeapSales(true), minHeapUnsold(false), 
      invoiceCsvFile("invoices.csv"), productsCsvFile("products.csv") {} // <-- CẬP NHẬT

SystemManager::~SystemManager() {
    for (Product* p : masterInventory) delete p;
    masterInventory.clear();
}

bool SystemManager::initializeData(const std::string& productsFile, const std::string& invoicesFile) {
    productsCsvFile = productsFile; // <-- CẬP NHẬT
    invoiceCsvFile = invoicesFile;
    bool pOk = loadMasterCSV(productsFile, masterInventory);
    bool iOk = loadInvoicesCSV(invoicesFile, invoiceDB);

    if (pOk) {
        for (Product* p : masterInventory) {
            barcodeTable.insert(p->barcode, p);
            skuTable.insert(p->productID, p);
            maxHeapSales.push(p);
            minHeapUnsold.push(p);
        }
        std::cout << "[THONG BAO] Da nap " << masterInventory.size() << " san pham tu " << productsFile << " vao RAM!\n";
    }

    if (iOk) {
        std::cout << "[THONG BAO] Da nap " << invoiceDB.getSize() << " hoa don tu " << invoicesFile << " vao RAM!\n";
    }

    return pOk || iOk;
}

// =============================================================================
// [THAO TAC 1] BAN HANG MOI & CAP NHAT COMPOSITION (Section 5.4)
// =============================================================================
bool SystemManager::processNewSale(const std::string& key, int quantity, const std::string& currentDate) {
    Product* p = searchByBarcode(key);
    if (!p) p = searchBySKU(key);

    if (!p) {
        std::cout << " [LOI] Khong tim thay san pham!\n";
        return false;
    }
    if (p->stockQuantity < quantity) {
        std::cout << " [LOI] Ton kho khong du!\n";
        return false;
    }

    p->stockQuantity -= quantity;
    p->totalSoldQuantity += quantity;
    p->lastSoldDate = currentDate;

    maxHeapSales.updateItem(p);
    minHeapUnsold.updateItem(p);

    // [CẬP NHẬT BỀN VỮNG XUỐNG FILE PRODUCTS.CSV]
    saveProductsCSV(productsCsvFile, masterInventory);

    std::cout << " [THANH CONG] Da ban " << quantity << " x " << p->name 
              << " | Thanh tien: " << std::fixed << std::setprecision(0) << (quantity * p->price) << " VND\n";
    return true;
}

// =============================================================================
// [THAO TAC 2 - MC1] TRA CUU CHINH XAC QUY MO LON (Custom Hash Table O(1))
// =============================================================================
Product* SystemManager::searchByBarcode(const std::string& barcode) {
    Product* p = nullptr;
    if (barcodeTable.get(barcode, p)) return p;
    return nullptr;
}

Product* SystemManager::searchBySKU(const std::string& sku) {
    Product* p = nullptr;
    if (skuTable.get(sku, p)) return p;
    return nullptr;
}

// =============================================================================
// [THAO TAC 3 - MC2] TRUY XUAT THEO KHOANG GIA & DUY NHAT 1 SP TON KHO THAP NHAT
// =============================================================================
std::vector<Product*> SystemManager::getProductsByPriceRange(double minPrice, double maxPrice) {
    if (minPrice > maxPrice) {
        std::swap(minPrice, maxPrice);
    }

    std::vector<Product*> result;
    for (Product* p : masterInventory) {
        if (p && p->price >= minPrice && p->price <= maxPrice) {
            result.push_back(p);
        }
    }

    std::sort(result.begin(), result.end(), [](const Product* a, const Product* b) {
        return a->price < b->price;
    });

    return result;
}

Product* SystemManager::getSingleLowestStockProduct() {
    if (masterInventory.empty()) return nullptr;

    Product* lowest = masterInventory[0];
    for (Product* p : masterInventory) {
        if (p && p->stockQuantity < lowest->stockQuantity) {
            lowest = p;
        }
    }
    return lowest;
}

// =============================================================================
// [THAO TAC 4 - TP1] TRA CUU HOA DON & XU LY DOI/TRA HANG TRONG 3 NGAY (Custom Hash Table O(1))
// =============================================================================
bool SystemManager::queryInvoice(const std::string& invoiceID, bool hasPermission, Invoice& result) {
    std::cout << "\n--- TRUY VAN HOA DON (TP1) ---" << std::endl;
    if (invoiceID.empty()) {
        std::cout << " Loi: Ma hoa don khong duoc de trong!" << std::endl;
        return false;
    }
    if (!hasPermission) {
        std::cout << " Loi: Ca truc nay thu ngan khong co quyen xem lich su." << std::endl;
        return false;
    }

    if (invoiceDB.get(invoiceID, result)) {
        struct tm timeinfo;
        char buffer[80];

#ifdef _WIN32
        localtime_s(&timeinfo, &result.purchase_time);
#else
        localtime_r(&result.purchase_time, &timeinfo);
#endif
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);

        std::cout << " TIM THAY HOA DON: " << result.invoiceID << std::endl;
        std::cout << "Thoi gian mua: " << buffer << std::endl;
        std::cout << "Trang thai: " << result.status << " | Tong tien: " << std::fixed << std::setprecision(0) << result.total_paid << " VND" << std::endl;
        std::cout << "Danh sach san pham:" << std::endl;
        for (const auto& p : result.products) {
            std::cout << "  - [" << p.productID << "] " << p.name 
                      << " | SL mua: " << p.invoiceQuantity << " (Da doi/tra: " << p.returnedQuantity << ")"
                      << " | Gia: " << std::fixed << std::setprecision(0) << p.price 
                      << " | Giam gia: " << p.discountApplied << " VND" << std::endl;
        }
        return true;
    } else {
        std::cout << " Loi: Khong tim thay hoa don (Ma khong khop)." << std::endl;
        return false;
    }
}

bool SystemManager::processReturnExchange(const std::string& invoiceID, const std::string& productID, const std::string& requestType, int returnQty) {
    std::cout << "\n--- XU LY DOI/TRA HANG (TP1) ---" << std::endl;
    
    Invoice inv;
    if (!invoiceDB.get(invoiceID, inv)) {
        std::cout << " CANH BAO DO: Khong tim thay hoa don " << invoiceID << std::endl;
        return false;
    }

    time_t currentTime = time(nullptr);
    char buffer[80];
    struct tm timeinfo;

#ifdef _WIN32
    localtime_s(&timeinfo, &currentTime);
#else
    localtime_r(&currentTime, &timeinfo);
#endif
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
    std::cout << "Thoi gian he thong hien tai: " << buffer << std::endl;

    double diffSeconds = difftime(currentTime, inv.purchase_time);
    if (diffSeconds > 3 * 24 * 60 * 60) {
        std::cout << " CANH BAO DO: Tu choi xu ly! Da qua han 3 ngay doi tra." << std::endl;
        return false;
    }

    Product* targetProduct = nullptr;
    for (auto& p : inv.products) {
        if (p.productID == productID) {
            targetProduct = &p;
            break;
        }
    }

    if (targetProduct == nullptr) {
        std::cout << " CANH BAO DO: Tu choi! San pham khong co trong bill nay." << std::endl;
        return false;
    }

    int availableToReturn = targetProduct->invoiceQuantity - targetProduct->returnedQuantity;
    if (returnQty > availableToReturn) {
        std::cout << " CANH BAO DO: So luong yeu counter vuot qua (Khach chi con " 
                  << availableToReturn << " mon co the doi tra)." << std::endl;
        return false;
    }

    double refundPerItem = targetProduct->price - targetProduct->discountApplied;
    double totalRefund = refundPerItem * returnQty;

    // 1. Cập nhật hóa đơn và lưu invoices.csv
    targetProduct->returnedQuantity += returnQty;
    inv.total_paid -= totalRefund; 
    if (inv.total_paid < 0) inv.total_paid = 0;

    invoiceDB.insert(inv.invoiceID, inv); 
    saveInvoicesCSV(invoiceCsvFile, invoiceDB);

    // 2. CẬP NHẬT KHO HÀNG (MASTER INVENTORY) VÀ LƯU PRODUCTS.CSV
    Product* invProduct = searchBySKU(productID);
    if (invProduct) {
        invProduct->stockQuantity += returnQty; // Tồn kho tăng lại
        invProduct->totalSoldQuantity -= returnQty; // Bán tích lũy giảm đi
        if (invProduct->totalSoldQuantity < 0) {
            invProduct->totalSoldQuantity = 0;
        }

        // Cập nhật lại chỉ mục trên 2 Heap
        maxHeapSales.updateItem(invProduct);
        minHeapUnsold.updateItem(invProduct);

        // Lưu bền vững vào products.csv
        saveProductsCSV(productsCsvFile, masterInventory);
    }

    std::cout << " XAC NHAN CHO PHEU " << (requestType == "Refund" ? "TRA HANG" : "DOI HANG") << std::endl;
    std::cout << "Mat hang: " << targetProduct->name << " x " << returnQty << std::endl;
    std::cout << "So tien hoan lai: " << std::fixed << std::setprecision(0) << totalRefund << " VND" << std::endl;
    std::cout << "Tong tien hoa don sau khi cap nhat: " << std::fixed << std::setprecision(0) << inv.total_paid << " VND" << std::endl;
    return true;
}

void SystemManager::printAllInvoices() {
    std::cout << "\n========================================================\n";
    std::cout << "      DANH SACH TAT CA HOA DON DANG LUU TRONG RAM       \n";
    std::cout << "========================================================\n";
    std::vector<Invoice> allInvoices = invoiceDB.getAllValues();
    if (allInvoices.empty()) {
        std::cout << " Chua co hoa don nao.\n";
        return;
    }
    for (const auto& inv : allInvoices) {
        std::cout << " BillID: " << inv.invoiceID 
                  << " | Tong tien: " << std::fixed << std::setprecision(0) << inv.total_paid 
                  << " VND | So mon: " << inv.products.size() << "\n";
    }
}

// =============================================================================
// [THAO TAC 5 - TP2] MAT HANG BAN CHAY NHAT (Custom Binary Max-Heap O(1) top)
// =============================================================================
void SystemManager::displayTopSelling(int k) {
    std::cout << "\n========================================================\n";
    std::cout << "     BANG XEP HANG TOP " << k << " SAN PHAM BAN CHAY NHAT (TP2)   \n";
    std::cout << "========================================================\n";

    BinaryHeap tempHeap = maxHeapSales;
    int rank = 1;
    while (!tempHeap.empty() && rank <= k) {
        Product* p = tempHeap.top();
        tempHeap.pop();
        if (p) {
            std::cout << " Top " << rank++ << ": " << p->name << " | Da ban: " << p->totalSoldQuantity << " cai\n";
        }
    }
}

// =============================================================================
// [THAO TAC 6 - TP3] HANG DONG VON / CHAM LUAN CHUYEN (Custom Binary Min-Heap O(1) top)
// =============================================================================
void SystemManager::displayStagnantInventory(int k) {
    std::cout << "\n========================================================\n";
    std::cout << "   CANH BAO TOP " << k << " SAN PHAM DONG VON / CHAM LUAN CHUYEN (TP3)\n";
    std::cout << "========================================================\n";

    BinaryHeap tempHeap = minHeapUnsold;
    int rank = 1;
    while (!tempHeap.empty() && rank <= k) {
        Product* p = tempHeap.top();
        tempHeap.pop();
        if (p) {
            double capitalGiam = p->stockQuantity * p->price;
            std::cout << " Top " << rank++ << ": " << p->name 
                      << " | Ngay ban cu nhat: " << p->lastSoldDate 
                      << " | Von dong: " << std::fixed << std::setprecision(0) << capitalGiam << " VND\n";
        }
    }
}