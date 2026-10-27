#ifndef SORTS_H
#define SORTS_H

#include <vector>
using namespace std;

// Key comparisons made by the comparison-based sorts. Reset before each run.
extern long long comparisons;

void insertionSort(vector<int>& a);
void selectionSort(vector<int>& a);
void bubbleSort(vector<int>& a);
void mergeSort(vector<int>& a);
void quickSort(vector<int>& a);
void heapSort(vector<int>& a);
void countingSort(vector<int>& a);
void radixSort(vector<int>& a);

#endif
