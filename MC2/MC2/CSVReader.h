#ifndef CSV_READER_H
#define CSV_READER_H

struct Product {
    char id[20];
    char name[100];
    int salePrice;
    int stock;
};

int readCSV(const char filename[], Product products[], int maxSize);

#endif
