#include <chrono>
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (arr[j] < pivot) {
                i++;
                std::swap(arr[i], arr[j]);
            }
        }
        std::swap(arr[i + 1], arr[high]);
        int pi = i + 1;
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    unsigned long size = 100000;
    srand(time(0));
    int* v1, * v2, * v3;
    auto start = high_resolution_clock::now();
    v1 = (int*)malloc(size * sizeof(int));
    v2 = (int*)malloc(size * sizeof(int));
    v3 = (int*)malloc(size * sizeof(int));
    randomVector(v1, size);
    randomVector(v2, size);

#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        v3[i] = v1[i] + v2[i];
    }

    // Sorting v3 using QuickSort in parallel
#pragma omp parallel
    {
#pragma omp single nowait
        quickSort(v3, 0, size - 1);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function (including sorting): " << duration.count() << " microseconds" << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
