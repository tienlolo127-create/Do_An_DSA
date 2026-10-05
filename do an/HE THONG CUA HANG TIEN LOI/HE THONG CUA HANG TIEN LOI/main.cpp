#include <iostream>
#include <limits>
#include <iomanip>
#include <ctime>
#include "SystemManager.h"

void showMenu() {
    std::cout << "\n========================================================\n";
    std::cout << "  HE THONG QUAN LY CUA HANG TAP HOA (DSA CORE SYSTEM)  \n";
    std::cout << "========================================================\n";
    std::cout << " 1. [BAN HANG MOI] Cap nhat giao dich & Position Map (Heap)\n";
    std::cout << " 2. [MC1] Tra cuu san pham theo Barcode hoac SKU (Hash Table)\n";
    std::cout << " 3. [MC2] Duyet san pham theo Khoang gia & SP Ton kho thap nhat (STL)\n";
    std::cout << " 4. [TP1] Truy van Hoa don & Doi/Tra hang 3 ngay (Hash Table)\n";
    std::cout << " 5. [TP2] Xem Bang xep hang Top san pham ban chay (Max-Heap)\n";
    std::cout << " 6. [TP3] Canh bao san pham Dong von / Cham luan chuyen (Min-Heap)\n";
    std::cout << " 0. Thoat chuong trinh\n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << " Chon thao tac (0-6): ";
}

int main() {
    SystemManager sys;

    if (!sys.initializeData("products.csv", "invoices.csv")) {
        std::cout << "[CANH BAO] Khong the khoi tao mot so du lieu tu CSV!\n";
    }

    int choice = -1;
    while (choice != 0) {
        showMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: { // Sale
                std::cout << "\n--- [THAO TAC 1 - BAN HANG MOI] CAP NHAT GIAO DICH & POSITION MAP ---\n";
                std::string key, date; int qty;
                std::cout << "Nhap Barcode hoac SKU: "; std::cin >> key;
                std::cout << "Nhap So luong ban: "; std::cin >> qty;
                std::cout << "Nhap Ngay ban (YYYY-MM-DD): "; std::cin >> date;
                sys.processNewSale(key, qty, date);
                break;
            }
            case 2: { // MC1
                std::cout << "\n--- [THAO TAC 2 - MC1] TRA CUU SAN PHAM (CUSTOM HASH TABLE) ---\n";
                std::cout << "Nhap Ma vach (Barcode) hoac SKU/ProductID: ";
                std::string key;
                std::cin >> key;

                Product* p = sys.searchByBarcode(key);
                if (!p) p = sys.searchBySKU(key);

                if (p) printProductDetails(p);
                else std::cout << " [LOI] Khong tim thay san pham!\n";
                break;
            }
            case 3: { // MC2
                std::cout << "\n--- [THAO TAC 3 - MC2] TRUY XUAT THEO KHOANG GIA & TON KHO THAP NHAT ---\n";
                std::cout << "1. Loc theo Khoang gia (Price Range)\n";
                std::cout << "2. Tim san pham co so luong ton kho thap nhat (Duy nhat 1 SP)\n";
                std::cout << "Chon (1-2): ";
                int subChoice; std::cin >> subChoice;

                if (subChoice == 1) {
                    double minP, maxP;
                    std::cout << "Nhap Min Price (VND): "; std::cin >> minP;
                    std::cout << "Nhap Max Price (VND): "; std::cin >> maxP;

                    auto list = sys.getProductsByPriceRange(minP, maxP);

                    if (list.empty()) {
                        std::cout << " [THONG BAO] Khong tim thay san pham nao trong khoang gia tu " 
                                  << std::fixed << std::setprecision(0) << minP << " den " << maxP << " VND!\n";
                    } else {
                        std::cout << "\n TIM THAY " << list.size() << " SAN PHAM PHU HOP KHOANG GIA:\n";
                        for (Product* p : list) {
                            std::cout << "  - [" << p->productID << "] " << p->name 
                                      << " | Gia: " << std::fixed << std::setprecision(0) << p->price << " VND"
                                      << " | Ton kho: " << p->stockQuantity << "\n";
                        }
                    }
                } else if (subChoice == 2) {
                    Product* lowest = sys.getSingleLowestStockProduct();

                    if (!lowest) {
                        std::cout << " [THONG BAO] Kho hang hien dang rong!\n";
                    } else {
                        std::cout << "\n SAN PHAM CO TON KHO THAP NHAT HE THONG (DUY NHAT 1 SP):\n";
                        printProductDetails(lowest);
                    }
                }
                break;
            }
            case 4: { // TP1
                std::cout << "\n--- [THAO TAC 4 - TP1] QUAN LY HOA DON & DOI/TRA HANG 3 NGAY (CUSTOM HASH TABLE) ---\n";
                std::cout << "1. Truy van Hoa don (Query Invoice)\n2. Thuc hien Doi/Tra hang (Return/Exchange)\n3. Xem tat ca Hoa don trong RAM\nChon (1-3): ";
                int subChoice; std::cin >> subChoice;

                if (subChoice == 1) {
                    std::string invID;
                    std::cout << "Nhap Ma Hoa don: "; std::cin >> invID;
                    Invoice inv;
                    sys.queryInvoice(invID, true, inv);
                } else if (subChoice == 2) {
                    std::string invID, prodID, type;
                    int qty;
                    std::cout << "Nhap Ma Hoa don: "; std::cin >> invID;
                    std::cout << "Nhap Ma San pham trong Bill: "; std::cin >> prodID;
                    std::cout << "Loai yeu cau (Refund/Exchange): "; std::cin >> type;
                    std::cout << "So luong tra: "; std::cin >> qty;

                    Invoice inv;
                    if (sys.queryInvoice(invID, true, inv)) {
                        sys.processReturnExchange(invID, prodID, type, qty);
                    }
                } else if (subChoice == 3) {
                    sys.printAllInvoices();
                }
                break;
            }
            case 5: { // TP2
                std::cout << "\n--- [THAO TAC 5 - TP2] TOP SAN PHAM BAN CHAY NHAT (INDEXED MAX-HEAP) ---\n";
                std::cout << "Nhap K: "; int k; std::cin >> k;
                sys.displayTopSelling(k);
                break;
            }
            case 6: { // TP3
                std::cout << "\n--- [THAO TAC 6 - TP3] SAN PHAM DONG VON / CHAM LUAN CHUYEN (INDEXED MIN-HEAP) ---\n";
                std::cout << "Nhap K: "; int k; std::cin >> k;
                sys.displayStagnantInventory(k);
                break;
            }
            case 0:
                std::cout << "\n[THONG BAO] Tam biet!\n";
                break;
        }
    }
    return 0;
}