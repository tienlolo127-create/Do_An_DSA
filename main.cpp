#include <iostream>
#include <limits>
#include <iomanip>
#include <ctime>
#include "SystemManager.h"

void showMenu() {
    std::cout << "\n========================================================\n";
    std::cout << "       HE THONG QUAN LY CUA HANG TAP HOA                \n";
    std::cout << "========================================================\n";
    std::cout << "  1. Cap nhat giao dich ban hang                        \n";
    std::cout << "  2. Tra cuu san pham                                   \n";
    std::cout << "  3. Tra cuu khoang gia & San pham ton kho thap nhat    \n";
    std::cout << "  4. Quan ly hoa don & Doi tra hang                     \n";
    std::cout << "  5. Top san pham ban chay                              \n";
    std::cout << "  6. Canh bao san pham dong von                         \n";
    std::cout << "  7. DEMO XUNG DOT                          \n";
    std::cout << "  0. Thoat chuong trinh                                 \n";
    std::cout << "--------------------------------------------------------\n";
    std::cout << " > Chon thao tac: ";
}

int main() {
    SystemManager sys;

    // DEBUG
    PROFILE_SCOPE("Nap 10,000 san pham va hoa don tu CSV", {
        if (!sys.initializeData("products.csv", "invoices.csv")) {
            std::cout << "\n[!] CANH BAO: Khong the khoi tao mot so du lieu tu CSV!\n";
        }
    });

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
                std::cout << "\n========================================================\n";
                std::cout << "            CAP NHAT GIAO DICH BAN HANG                 \n";
                std::cout << "========================================================\n";
                std::string key, date; int qty;
                std::cout << " + Nhap Barcode hoac SKU   : "; std::cin >> key;
                std::cout << " + Nhap So luong ban       : "; std::cin >> qty;
                std::cout << " + Nhap Ngay ban           : "; std::cin >> date;
                std::cout << "--------------------------------------------------------\n";
                //DEBUG
                PROFILE_SCOPE("Cap nhat giao dich ban hang", {
                    sys.processNewSale(key, qty, date);
                });
                break;
            }
            case 2: { // MC1
                std::cout << "\n========================================================\n";
                std::cout << "                TRA CUU SAN PHAM                        \n";
                std::cout << "========================================================\n";
                std::cout << " + Nhap Ma vach hoac SKU/ProductID: ";
                std::string key;
                std::cin >> key;

                Product* p = nullptr;
                //DEBUG
                PROFILE_SCOPE("Truy van Hash Table", {
                    p = sys.searchByBarcode(key);
                    if (!p) p = sys.searchBySKU(key);
                });

                std::cout << "--------------------------------------------------------\n";
                if (p) printProductDetails(p);
                else std::cout << " [X] Khong tim thay san pham!\n";
                break;
            }
            case 3: { // MC2
                std::cout << "\n========================================================\n";
                std::cout << "      TRUY XUAT THEO KHOANG GIA & TON KHO THAP NHAT     \n";
                std::cout << "========================================================\n";
                std::cout << "  1. Loc theo Khoang gia\n";
                std::cout << "  2. Tim san pham co so luong ton kho thap nhat\n";
                std::cout << "--------------------------------------------------------\n";
                std::cout << " > Chon thao tac: ";
                int subChoice; std::cin >> subChoice;

                if (subChoice == 1) {
                    double minP, maxP;
                    std::cout << "\n + Nhap Gia toi thieu : "; std::cin >> minP;
                    std::cout << " + Nhap Gia toi da    : "; std::cin >> maxP;
                    std::cout << "--------------------------------------------------------\n";

                    std::vector<Product*> list;

                    // DEBUG
                    PROFILE_SCOPE("Loc san pham theo khoang gia", {
                        list = sys.getProductsByPriceRange(minP, maxP);
                    });

                    if (list.empty()) {
                        std::cout << " [!] Khong tim thay san pham nao trong khoang gia tu " 
                                  << std::fixed << std::setprecision(0) << minP << " den " << maxP << " VND!\n";
                    } else {
                        std::cout << " [V] TIM THAY " << list.size() << " SAN PHAM PHU HOP KHOANG GIA:\n\n";
                        for (Product* p : list) {
                            std::cout << "   - [" << p->productID << "] " << p->name 
                                      << " | Gia: " << std::fixed << std::setprecision(0) << p->price << " VND"
                                      << " | Ton kho: " << p->stockQuantity << "\n";
                        }
                    }
                } else if (subChoice == 2) {
                    Product* lowest = nullptr;
                    // DEBUG
                    PROFILE_SCOPE("Tim san pham ton kho thap nhat", {
                        lowest = sys.getSingleLowestStockProduct();
                    });
                    std::cout << "--------------------------------------------------------\n";

                    if (!lowest) {
                        std::cout << " [!] Kho hang hien dang rong!\n";
                    } else {
                        std::cout << " [V] SAN PHAM CO TON KHO THAP NHAT HE THONG:\n\n";
                        printProductDetails(lowest);
                    }
                }
                break;
            }
            case 4: { // TP1
                std::cout << "\n========================================================\n";
                std::cout << "          QUAN LY HOA DON & DOI TRA HANG                \n";
                std::cout << "========================================================\n";
                std::cout << "  1. Truy van Hoa don\n";
                std::cout << "  2. Thuc hien Doi tra hang\n";
                std::cout << "  3. Xem tat ca Hoa don trong RAM\n";
                std::cout << "--------------------------------------------------------\n";
                std::cout << " > Chon thao tac: ";
                int subChoice; std::cin >> subChoice;

                if (subChoice == 1) {
                    std::string invID;
                    std::cout << "\n + Nhap Ma Hoa don: "; std::cin >> invID;
                    std::cout << "--------------------------------------------------------\n";
                    Invoice inv;
                    sys.queryInvoice(invID, true, inv);
                } else if (subChoice == 2) {
                    std::string invID, prodID, type;
                    int qty;
                    std::cout << "\n + Nhap Ma Hoa don       : "; std::cin >> invID;
                    std::cout << " + Nhap Ma San pham      : "; std::cin >> prodID;
                    std::cout << " + Loai yeu cau          : "; std::cin >> type;
                    std::cout << " + So luong tra          : "; std::cin >> qty;
                    std::cout << "--------------------------------------------------------\n";

                    Invoice inv;
                    if (sys.queryInvoice(invID, true, inv)) {
                        //DEBUG
                        PROFILE_SCOPE("Thuc hien doi tra hang", {
                            sys.processReturnExchange(invID, prodID, type, qty);
                        });
                    }
                } else if (subChoice == 3) {
                    std::cout << "--------------------------------------------------------\n";
                    sys.printAllInvoices();
                }
                break;
            }
            case 5: { // TP2
                std::cout << "\n========================================================\n";
                std::cout << "             TOP SAN PHAM BAN CHAY NHAT                 \n";
                std::cout << "========================================================\n";
                std::cout << " + Nhap so luong san pham K: "; int k; std::cin >> k;
                std::cout << "--------------------------------------------------------\n";
                //DEBUG
                PROFILE_SCOPE("Lay Top K Max-Heap", {
                    sys.displayTopSelling(k);
                });
                break;
            }
            case 6: { // TP3
                std::cout << "\n========================================================\n";
                std::cout << "              SAN PHAM DONG VON                         \n";
                std::cout << "========================================================\n";
                std::cout << " + Nhap so luong san pham K: "; int k; std::cin >> k;
                std::cout << "--------------------------------------------------------\n";
                //DEBUG
                PROFILE_SCOPE("Lay Top K San pham dong von", {
                    sys.displayStagnantInventory(k);
                });
                break;
            }
            //  CASE 7 - XUNG DOT TP2 <-> TP3

            case 7: {
                std::cout<< "\n--- [THAO TAC 7 - DEMO XUNG DOT " << "TP2 <-> TP3] ---\n";               

                std::string key;
                std::string date;

                int qty;

                std::cout<< "Nhap Barcode hoac SKU: ";                    

                std::cin >> key;

                std::cout << "Nhap So luong ban    : ";
                   
                std::cin >> qty;

                std::cout  << "Nhap Ngay ban (YYYY-MM-DD): ";
                  
                std::cin >> date;
                
                //DEBUG
                PROFILE_SCOPE("Thuc thi Demo Xung dot", {
                    sys.demonstrateConflict(key, qty, date);
                });

                break;
            }
            case 0:
                std::cout << "\n========================================================\n";
                std::cout << "                 THOAT CHUONG TRINH!               \n";
                std::cout << "========================================================\n";
                PROFILE_SCOPE("Luu du lieu vao CSV", {
                    sys.saveData("products.csv");
                });
                break;
        }
    }
    return 0;
}