#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
using namespace std;

struct Product {
    string productID;
    string name;
    int quantity;
    int returned_quantity;
    double unit_price;
    double discount_applied;
};

struct Invoice {
    string invoiceID;
    time_t purchase_time;
    vector<Product> products;
    double total_paid;
    string status;
};

template<typename K, typename V>
class CustomHashMap {
private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
        HashNode(K k, V v) : key(k), value(v), next(nullptr) {}
    };
    HashNode** table;
    int capacity;

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) {
            hash = (hash * 31 + c) % capacity;
        }
        return hash;
    }

public:
    CustomHashMap(int cap = 100) : capacity(cap) {
        table = new HashNode*[capacity]();
    }

    void insert(K key, V value) {
        int hashIndex = hashFunction(key);
        HashNode* prev = nullptr;
        HashNode* entry = table[hashIndex];

        while (entry != nullptr && entry->key != key) {
            prev = entry;
            entry = entry->next;
        }

        if (entry == nullptr) {
            entry = new HashNode(key, value);
            if (prev == nullptr) {
                table[hashIndex] = entry;
            } else {
                prev->next = entry;
            }
        } else {
            entry->value = value;
        }
    }

    bool get(K key, V& out_value) {
        int hashIndex = hashFunction(key);
        HashNode* entry = table[hashIndex];
        while (entry != nullptr) {
            if (entry->key == key) {
                out_value = entry->value;
                return true;
            }
            entry = entry->next;
        }
        return false;
    }
    
    vector<V> getAllValues() {
        vector<V> values;
        for (int i = 0; i < capacity; i++) {
            HashNode* entry = table[i];
            while (entry != nullptr) {
                values.push_back(entry->value);
                entry = entry->next;
            }
        }
        return values;
    }

    ~CustomHashMap() {
        for (int i = 0; i < capacity; i++) {
            HashNode* entry = table[i];
            while (entry != nullptr) {
                HashNode* prev = entry;
                entry = entry->next;
                delete prev;
            }
        }
        delete[] table;
    }
};

class POSSystem {
private:
    CustomHashMap<string, Invoice> invoiceDB;

public:
    void loadInvoicesFromCSVFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Không mở được file " << filename << endl;
            return;
        }

        string line;
        Invoice currentInv;
        bool hasInvoice = false;

        while (getline(file, line)) {
            if (line.empty()) continue; 

            stringstream ss(line);
            string type;
            getline(ss, type, ','); 

            if (type == "INVOICE") {
                if (hasInvoice) {
                    invoiceDB.insert(currentInv.invoiceID, currentInv);
                }
                
                currentInv = Invoice(); 
                getline(ss, currentInv.invoiceID, ',');
                
                string timeStr, totalStr;
                getline(ss, timeStr, ',');
                currentInv.purchase_time = stoll(timeStr); 
                
                getline(ss, totalStr, ',');
                currentInv.total_paid = stod(totalStr); 
                
                getline(ss, currentInv.status, ',');
                
                hasInvoice = true;
            } 
            else if (type == "PRODUCT" && hasInvoice) {
                Product p;
                getline(ss, p.productID, ',');
                getline(ss, p.name, ',');
                
                string qty, retQty, price, disc;
                getline(ss, qty, ','); p.quantity = stoi(qty);
                getline(ss, retQty, ','); p.returned_quantity = stoi(retQty);
                getline(ss, price, ','); p.unit_price = stod(price);
                getline(ss, disc, ','); p.discount_applied = stod(disc);
                
                currentInv.products.push_back(p);
            }
        }
        if (hasInvoice) {
            invoiceDB.insert(currentInv.invoiceID, currentInv);
        }
        
        file.close();
    }

    void saveInvoicesToCSVFile(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cout << "Không lưu được file " << filename << endl;
            return;
        }

        vector<Invoice> allInvoices = invoiceDB.getAllValues();
        for (const auto& inv : allInvoices) {
            file << "INVOICE," << inv.invoiceID << "," << inv.purchase_time << "," 
                 << inv.total_paid << "," << inv.status << "\n";
            for (const auto& p : inv.products) {
                file << "PRODUCT," << p.productID << "," << p.name << "," 
                     << p.quantity << "," << p.returned_quantity << "," 
                     << p.unit_price << "," << p.discount_applied << "\n";
            }
            file << "\n";
        }
        file.close();
    }

    bool queryInvoice(string invoiceID, bool hasPermission, Invoice& result) {
        cout << "\n--- TRUY VẤN HÓA ĐƠN ---" << endl;
        if (invoiceID.empty()) {
            cout << " Lỗi: Mã hóa đơn không được để trống!" << endl;
            return false;
        }
        if (!hasPermission) {
            cout << " Lỗi: Ca trực này thu ngân không có quyền xem lịch sử." << endl;
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

            cout << " TÌM THẤY HÓA ĐƠN: " << result.invoiceID << endl;
            cout << "Thời gian mua: " << buffer << endl;
            cout << "Trạng thái: " << result.status << " | Tổng tiền: " << result.total_paid << " VND" << endl;
            cout << "Danh sách sản phẩm:" << endl;
            for (const auto& p : result.products) {
                cout << "  - [" << p.productID << "] " << p.name 
                     << " | SL: " << p.quantity << " (Đã đổi/trả: " << p.returned_quantity << ")"
                     << " | Giá: " << p.unit_price 
                     << " | Giảm giá: " << p.discount_applied << " VND" << endl;
            }
            return true;
        } else {
            cout << " Lỗi: Không tìm thấy hóa đơn (Mã không khớp)." << endl;
            return false;
        }
    }

    void processReturnExchange(Invoice& inv, string productID, string requestType, int returnQty) {
        cout << "\n--- XỬ LÝ ĐỔI/TRẢ HÀNG ---" << endl;
        
        time_t currentTime = time(nullptr);
        struct tm timeinfo;
        char buffer[80];
#ifdef _WIN32
        localtime_s(&timeinfo, &currentTime);
#else
        localtime_r(&currentTime, &timeinfo);
#endif
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
        cout << "Thời gian hệ thống hiện tại: " << buffer << endl;

        double diffSeconds = difftime(currentTime, inv.purchase_time);
        if (diffSeconds > 3 * 24 * 60 * 60) {
            cout << " CẢNH BÁO ĐỎ: Từ chối xử lý! Đã quá hạn 3 ngày đổi trả." << endl;
            return;
        }

        Product* targetProduct = nullptr;
        for (auto& p : inv.products) {
            if (p.productID == productID) {
                targetProduct = &p;
                break;
            }
        }

        if (targetProduct == nullptr) {
            cout << " CẢNH BÁO ĐỎ: Từ chối! Sản phẩm không có trong bill này." << endl;
            return;
        }

        int availableToReturn = targetProduct->quantity - targetProduct->returned_quantity;
        if (returnQty > availableToReturn) {
            cout << " CẢNH BÁO ĐỎ: Số lượng yêu cầu vượt quá (Khách chỉ còn " 
                 << availableToReturn << " món có thể đổi trả)." << endl;
            return;
        }

        double refundPerItem = targetProduct->unit_price - targetProduct->discount_applied;
        double totalRefund = refundPerItem * returnQty;

        targetProduct->returned_quantity += returnQty;
        
        inv.total_paid -= totalRefund; 

        invoiceDB.insert(inv.invoiceID, inv); 

        cout << " XÁC NHẬN CHO PHÉP " << (requestType == "Refund" ? "TRẢ HÀNG" : "ĐỔI HÀNG") << endl;
        cout << "Mặt hàng: " << targetProduct->name << " x " << returnQty << endl;
        cout << "Số tiền hoàn lại: " << totalRefund << " VND" << endl;
        cout << "Tổng tiền hóa đơn sau khi cập nhật: " << inv.total_paid << " VND" << endl;
    }
};

int main() {
    POSSystem pos;
    pos.loadInvoicesFromCSVFile("data.csv");

    Invoice currentInvoice;
    if (pos.queryInvoice("BILL12345", true, currentInvoice)) {
        pos.processReturnExchange(currentInvoice, "P001", "Refund", 1);
    }
    
    pos.saveInvoicesToCSVFile("data.csv");
}