#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include "CSVReader.h"

using namespace std;

int readCSV(const char filename[], Product products[], int maxSize) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Khong the mo file: " << filename << endl;
        return 0;
    }

    string line;
    getline(file, line); // Bo qua header

    int count = 0;

    while (getline(file, line) && count < maxSize) {
        if (line.empty()) continue;

        stringstream ss(line);
        string id, name, salePrice, stock;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, salePrice, ',');
        getline(ss, stock, ',');

        strncpy(products[count].id, id.c_str(), sizeof(products[count].id) - 1);
        products[count].id[sizeof(products[count].id) - 1] = '\0';

        strncpy(products[count].name, name.c_str(), sizeof(products[count].name) - 1);
        products[count].name[sizeof(products[count].name) - 1] = '\0';

        products[count].salePrice = atoi(salePrice.c_str());
        products[count].stock = atoi(stock.c_str());
        count++;
    }

    file.close();
    return count;
}
