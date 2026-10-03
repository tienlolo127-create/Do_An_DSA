#ifndef DATALOADER_H
#define DATALOADER_H

#include <string>
#include <vector>
#include "Product.h"
#include "HashTable.h"

bool loadMasterCSV(const std::string& filename, std::vector<Product*>& inventory);
bool saveProductsCSV(const std::string& filename, const std::vector<Product*>& inventory); // <-- MỚI THÊM
bool loadInvoicesCSV(const std::string& filename, CustomHashTable<std::string, Invoice>& invoiceDB);
bool saveInvoicesCSV(const std::string& filename, const CustomHashTable<std::string, Invoice>& invoiceDB);

#endif
