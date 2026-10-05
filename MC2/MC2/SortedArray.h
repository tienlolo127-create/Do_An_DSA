#ifndef SORTED_ARRAY_H
#define SORTED_ARRAY_H

#include "CSVReader.h"

void buildSortedArray(Product products[], int index[], int n);
void rangeQuery(Product products[], int index[], int n, int minPrice, int maxPrice);

#endif
