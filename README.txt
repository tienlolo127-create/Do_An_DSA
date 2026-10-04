
MC1 - Tra cứu sản phẩm theo mã vạch
1. CAU TRUC 
Thu muc: DSA/
├── products.csv   : Tep du lieu ban ghi hang hoa ban dau (CSV)
├── Product.h      : Khai bao struct Product va ham hien thi
├── Product.cpp    : Dinh nghia ham in chi tiet san pham
├── HashTable.h    : Khai bao Bang bam (Hash Table) & Node danh sach
├── HashTable.cpp  : Cai dat ham bam FNV-1a, Rehash, Insert, Search
└── main.cpp       : Nap CSV, tao Index O(1) va giao dien CLI

2. CHUC NANG CHINH (MAIN OPERATIONS)
-------------------------------------------------------------------
- Nap du lieu     : Doc tep products.csv va khoi tao bo nho trong.
- Danh chi muc    : Luu con tro san pham vao 2 Bang bam (Barcode & SKU).
- Tra cuu O(1)    : Tim kiem tuc thi theo Ma vach (Barcode) hoac Ma SKU.
- Hien thi        : In Ten san pham, Gia ban, Ton kho, Vi tri tren ke.
- Rehash tu dong  : Tu dong mo rong bang khi Load Factor > 0.75.

3. HUONG DAN BIEN DICH VA CHAY TREN TERMINAL (VS CODE)
Buoc 1: Mo Terminal va chuyen vao thu muc du an
        cd DSA

Buoc 2: Bien dich va chay bang 1 cau lenh gop:
        g++ -std=c++11 *.cpp -o mc1_lookup.exe; .\mc1_lookup.exe

4. VI DU INPUT & OUTPUT THUC TE
VI DU 1: Tra cuu theo Ma vach (Barcode)
[INPUT]  : 8935001234567
[OUTPUT] :
============================================
        KET QUA TRA CUU HANG HOA (MC1)       
============================================
 Ma vach (Barcode): 8935001234567
 Ma san pham (SKU) : SKU-SNACK-001
 Ten san pham     : Banh Oishi Tom Cay 80g
 Gia ban          : 12000 VND
 Ton kho hien tai : 150 don vi
 Vi tri tren ke   : Ke A1-02
============================================

VI DU 2: Tra cuu theo Ma san pham (SKU) 
[INPUT]  : SKU-MILK-102
[OUTPUT] :
============================================
        KET QUA TRA CUU HANG HOA (MC1)       
============================================
 Ma vach (Barcode): 8934567890123
 Ma san pham (SKU) : SKU-MILK-102
 Ten san pham     : Sua Tuoi Vinamilk Co Duong 1L
 Gia ban          : 38000 VND
 Ton kho hien tai : 42 don vi
 Vi tri tren ke   : Ke B2-05
============================================

VI DU 3: Ma khong ton tai trong he thong (Edge Case)
[INPUT]  : 9999999999999
[OUTPUT] : [LOI] Khong tim thay san pham trong he thong!

VI DU 4: Thoat chuong trinh
[INPUT]  : exit
[OUTPUT] : Da thoat chuong trinh tra cuu MC1 thanh cong.