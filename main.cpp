// benchmark driver
// writes one row per run to results.csv

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <cstdlib>
#include "sorts.h"

using namespace std;

const int REPS = 5;

// sizes for O(n^2) sorts and faster sorts
const int SLOW_SIZES[] = {1000, 2000, 4000, 8000, 16000, 32000};
const int FAST_SIZES[] = {1000, 2000, 4000, 8000, 16000, 32000, 64000,
                          128000, 256000, 512000, 1024000};
const int NUM_SLOW_SIZES = 6;
const int NUM_FAST_SIZES = 11;

const string INPUT_TYPES[] = {"sorted", "reversed", "random", "equal", "mergeworst"};
const int NUM_INPUT_TYPES = 5;

struct Algorithm {
    string name;
    void (*sort)(vector<int>&);
    bool slow; // O(n^2) so small sizes only
};

const Algorithm ALGORITHMS[] = {
    {"insertion", insertionSort, true},
    {"selection", selectionSort, true},
    {"bubble",    bubbleSort,    true},
    {"merge",     mergeSort,     false},
    {"quick",     quickSort,     false},
    {"heap",      heapSort,      false},
    {"counting",  countingSort,  false},
    {"radix",     radixSort,     false},
};
const int NUM_ALGORITHMS = 8;

// input generation
// values in [0, n-1]

int randomInRange(int lo, int hi)
{
    long long r = (long long)rand() * (RAND_MAX + 1LL) + rand();
    return lo + (int)(r % (hi - lo + 1));
}

// rearranges sorted a to force max merge sort comparisons
// left half gets even positions
// right half gets odd positions
void makeMergeWorst(vector<int>& a)
{
    int n = a.size();
    if (n <= 1)
        return;
    vector<int> left, right;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0)
            left.push_back(a[i]);
        else
            right.push_back(a[i]);
    }
    // merge sort left half has (n+1)/2 elements
    // same as number of even positions
    makeMergeWorst(left);
    makeMergeWorst(right);
    for (int i = 0; i < (int)left.size(); i++)
        a[i] = left[i];
    for (int i = 0; i < (int)right.size(); i++)
        a[left.size() + i] = right[i];
}

vector<int> makeInput(string type, int n)
{
    vector<int> a(n);
    if (type == "sorted") {
        for (int i = 0; i < n; i++)
            a[i] = i;
    } else if (type == "reversed") {
        for (int i = 0; i < n; i++)
            a[i] = n - 1 - i;
    } else if (type == "random") {
        for (int i = 0; i < n; i++)
            a[i] = randomInRange(0, n - 1);
    } else if (type == "equal") {
        for (int i = 0; i < n; i++)
            a[i] = n / 2;
    } else if (type == "mergeworst") {
        for (int i = 0; i < n; i++)
            a[i] = i;
        makeMergeWorst(a);
    }
    return a;
}

bool isSorted(vector<int>& a)
{
    for (int i = 1; i < (int)a.size(); i++)
        if (a[i - 1] > a[i])
            return false;
    return true;
}

int main()
{
    ofstream out("results.csv");
    out << "algorithm,input,n,rep,time_ms,comparisons\n";

    for (int t = 0; t < NUM_INPUT_TYPES; t++) {
        string type = INPUT_TYPES[t];
        for (int s = 0; s < NUM_FAST_SIZES; s++) {
            int n = FAST_SIZES[s];
            bool slowSize = n <= SLOW_SIZES[NUM_SLOW_SIZES - 1];

            for (int rep = 0; rep < REPS; rep++) {
                // same seed gives same input for every algorithm
                srand(t * 1000003 + n * 31 + rep);
                vector<int> input = makeInput(type, n);

                for (int k = 0; k < NUM_ALGORITHMS; k++) {
                    Algorithm alg = ALGORITHMS[k];
                    // quicksort is O(n^2) on equal input
                    bool slow = alg.slow || (alg.name == "quick" && type == "equal");
                    if (slow && !slowSize)
                        continue;

                    vector<int> a = input;
                    comparisons = 0;

                    auto start = chrono::steady_clock::now();
                    alg.sort(a);
                    auto stop = chrono::steady_clock::now();
                    double ms = chrono::duration<double, milli>(stop - start).count();

                    if (!isSorted(a)) {
                        cout << "ERROR: " << alg.name << " failed on " << type
                             << " n=" << n << endl;
                        return 1;
                    }

                    out << alg.name << "," << type << "," << n << "," << rep << ","
                        << ms << "," << comparisons << "\n";
                }
            }
            out.flush();
            cout << type << " n=" << n << " done" << endl;
        }
    }

    cout << "Wrote results.csv" << endl;
    return 0;
}
