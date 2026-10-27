#include "sorts.h"
#include <cstdlib>

using namespace std;

long long comparisons = 0;

// Counted comparison: x < y
static bool lessThan(int x, int y)
{
    comparisons++;
    return x < y;
}

static void swapValues(int& x, int& y)
{
    int t = x;
    x = y;
    y = t;
}

// ---------- Insertion sort ----------

void insertionSort(vector<int>& a)
{
    int n = a.size();
    for (int i = 1; i < n; i++) {
        int key = a[i];
        int j = i - 1;
        while (j >= 0 && lessThan(key, a[j])) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

// ---------- Selection sort ----------

void selectionSort(vector<int>& a)
{
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < n; j++) {
            if (lessThan(a[j], a[minIndex]))
                minIndex = j;
        }
        if (minIndex != i)
            swapValues(a[i], a[minIndex]);
    }
}

// ---------- Bubble sort ----------
// Stops early if a pass makes no swaps.

void bubbleSort(vector<int>& a)
{
    int n = a.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - 1 - i; j++) {
            if (lessThan(a[j + 1], a[j])) {
                swapValues(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped)
            break;
    }
}

// ---------- Merge sort ----------

static void merge(vector<int>& a, vector<int>& temp, int lo, int mid, int hi)
{
    int i = lo;
    int j = mid + 1;
    int k = lo;
    while (i <= mid && j <= hi) {
        if (lessThan(a[j], a[i]))
            temp[k++] = a[j++];
        else
            temp[k++] = a[i++];
    }
    while (i <= mid)
        temp[k++] = a[i++];
    while (j <= hi)
        temp[k++] = a[j++];
    for (k = lo; k <= hi; k++)
        a[k] = temp[k];
}

static void mergeSortRange(vector<int>& a, vector<int>& temp, int lo, int hi)
{
    if (lo >= hi)
        return;
    int mid = (lo + hi) / 2;
    mergeSortRange(a, temp, lo, mid);
    mergeSortRange(a, temp, mid + 1, hi);
    merge(a, temp, lo, mid, hi);
}

void mergeSort(vector<int>& a)
{
    vector<int> temp(a.size());
    mergeSortRange(a, temp, 0, (int)a.size() - 1);
}

// ---------- Quicksort ----------
// Random pivot + Hoare partition. Random pivot avoids bad cases on
// sorted/reversed input; Hoare handles many duplicates well.

// Random int in [lo, hi]. rand() alone is too small on Windows (max 32767).
static int randomInRange(int lo, int hi)
{
    long long r = (long long)rand() * (RAND_MAX + 1LL) + rand();
    return lo + (int)(r % (hi - lo + 1));
}

static int partition(vector<int>& a, int lo, int hi)
{
    swapValues(a[lo], a[randomInRange(lo, hi)]);
    int pivot = a[lo];
    int i = lo - 1;
    int j = hi + 1;
    while (true) {
        do {
            j--;
        } while (lessThan(pivot, a[j]));
        do {
            i++;
        } while (lessThan(a[i], pivot));
        if (i >= j)
            return j;
        swapValues(a[i], a[j]);
    }
}

static void quickSortRange(vector<int>& a, int lo, int hi)
{
    if (lo >= hi)
        return;
    int p = partition(a, lo, hi);
    quickSortRange(a, lo, p);
    quickSortRange(a, p + 1, hi);
}

void quickSort(vector<int>& a)
{
    quickSortRange(a, 0, (int)a.size() - 1);
}

// ---------- Heapsort ----------

// Max-heap sift down of a[i] within a[0..size-1].
static void siftDown(vector<int>& a, int i, int size)
{
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = i;
        if (left < size && lessThan(a[largest], a[left]))
            largest = left;
        if (right < size && lessThan(a[largest], a[right]))
            largest = right;
        if (largest == i)
            return;
        swapValues(a[i], a[largest]);
        i = largest;
    }
}

void heapSort(vector<int>& a)
{
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--)
        siftDown(a, i, n);
    for (int end = n - 1; end > 0; end--) {
        swapValues(a[0], a[end]);
        siftDown(a, 0, end);
    }
}

// ---------- Counting sort ----------
// Values assumed in [0, max]. Stable.

void countingSort(vector<int>& a)
{
    int n = a.size();
    if (n == 0)
        return;
    int maxValue = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > maxValue)
            maxValue = a[i];

    vector<int> count(maxValue + 1, 0);
    for (int i = 0; i < n; i++)
        count[a[i]]++;
    for (int v = 1; v <= maxValue; v++)
        count[v] += count[v - 1];

    vector<int> output(n);
    for (int i = n - 1; i >= 0; i--) {
        count[a[i]]--;
        output[count[a[i]]] = a[i];
    }
    a = output;
}

// ---------- Radix sort ----------
// LSD, base 10. Stable counting sort on each digit.

static void countingSortByDigit(vector<int>& a, int place)
{
    int n = a.size();
    int count[10] = {0};
    for (int i = 0; i < n; i++)
        count[(a[i] / place) % 10]++;
    for (int d = 1; d < 10; d++)
        count[d] += count[d - 1];

    vector<int> output(n);
    for (int i = n - 1; i >= 0; i--) {
        int d = (a[i] / place) % 10;
        count[d]--;
        output[count[d]] = a[i];
    }
    a = output;
}

void radixSort(vector<int>& a)
{
    int n = a.size();
    if (n == 0)
        return;
    int maxValue = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > maxValue)
            maxValue = a[i];

    for (long long place = 1; maxValue / place > 0; place *= 10)
        countingSortByDigit(a, (int)place);
}
