#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

// Utility function to generate a random array of given size
vector<int> generateRandomArray(int size) {
    vector<int> arr(size);
    srand(time(nullptr));
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % 1000; // Generating random numbers between 0 and 999
    }
    return arr;
}

// Sequential Bubble Sort
void bubbleSortSequential(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Parallel Bubble Sort
void bubbleSortParallel(vector<int>& arr) {
    int n = arr.size();
    #pragma omp parallel
    {
        for (int i = 0; i < n - 1; ++i) {
            #pragma omp for
            for (int j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    #pragma omp critical
                    {
                        swap(arr[j], arr[j + 1]);
                    }
                }
            }
        }
    }
}

// Merge function for Merge Sort
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

// Sequential Merge Sort
void mergeSortSequential(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSequential(arr, left, mid);
        mergeSortSequential(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

// Parallel Merge Sort
void mergeSortParallel(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        #pragma omp parallel sections
        {
            #pragma omp section
            mergeSortParallel(arr, left, mid);
            #pragma omp section
            mergeSortParallel(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}

// Utility function to print array elements
void printArray(const vector<int>& arr) {
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;
}

int main() {
    const int SIZE = 10; // Size of the array
    vector<int> arr = generateRandomArray(SIZE);
    vector<int> arrSequential = arr; // Copy of the original array for sequential sorting

    cout << "Original Array:" << endl;
    printArray(arr); // Print original array

    // Perform sequential Bubble Sort
    bubbleSortSequential(arrSequential);

    // Print sorted array using sequential Bubble Sort
    cout << "Sorted Array (Sequential Bubble Sort):" << endl;
    printArray(arrSequential);

    // Perform parallel Bubble Sort
    bubbleSortParallel(arr);

    // Print sorted array using parallel Bubble Sort
    cout << "Sorted Array (Parallel Bubble Sort):" << endl;
    printArray(arr);

    // Perform sequential Merge Sort
    mergeSortSequential(arr, 0, SIZE - 1);

    // Print sorted array using sequential Merge Sort
    cout << "Sorted Array (Sequential Merge Sort):" << endl;
    printArray(arr);

    // Perform parallel Merge Sort
    mergeSortParallel(arr, 0, SIZE - 1);

    // Print sorted array using parallel Merge Sort
    cout << "Sorted Array (Parallel Merge Sort):" << endl;
    printArray(arr);

    return 0;
}
