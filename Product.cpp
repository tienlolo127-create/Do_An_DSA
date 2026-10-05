#include "Product.h"
#include <iomanip>

void printProductDetails(const Product* p) {
    if (!p) {
        std::cout << "[LOI] Khong tim thay san pham trong he thong!\n";
        return;
    }
    std::cout << "\n============================================\n";
    std::cout << "        KET QUA TRA CUU HANG HOA (MC1)       \n";
    std::cout << "============================================\n";
    std::cout << " Ma vach (Barcode): " << p->barcode << "\n";
    std::cout << " Ma san pham (SKU) : " << p->sku << "\n";
    std::cout << " Ten san pham     : " << p->name << "\n";
    std::cout << " Gia ban          : " << std::fixed << std::setprecision(0) << p->price << " VND\n";
    std::cout << " Ton kho hien tai : " << p->stockQuantity << " don vi\n";
    std::cout << " Vi tri tren ke   : " << p->shelfLocation << "\n";
    std::cout << "============================================\n\n";
}
