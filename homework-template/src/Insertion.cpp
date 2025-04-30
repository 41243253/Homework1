#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring> 
#include <chrono>    // 時間測量功能
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono; // 方便使用 steady_clock

// 插入元素到已經排序好的串列中
template <class T>
void Insert(const T& e, T* a, int i) {
    a[0] = e;
    while (e < a[i]) {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

// 插入排序法
template <class T>
void InsertionSort(T* a, const int n) {
    for (int j = 2; j <= n; j++) {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

// 顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

int main() {
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;

    if (mode == 1) {
        // Average Case: 從 testdata1000.txt 讀入 n 筆資料，重複排序 2000 次取平均
        int* orig = new int[n + 1];
        ifstream fin("testdata1000.txt");
        if (!fin) {
            cerr << "無法開啟 testdata1000.txt\n";
            delete[] orig;
            return 1;
        }
        for (int i = 1; i <= n; ++i) {
            fin >> orig[i];
        }
        fin.close();

        int* arr = new int[n + 1];
        printMemoryUsage();

        long long totalDuration = 0;
        for (int k = 0; k < 2000; ++k) {
            // 複製原始資料到工作陣列
            memcpy(arr + 1, orig + 1, n * sizeof(int));

            auto start = steady_clock::now();
            InsertionSort(arr, n);
            auto end = steady_clock::now();
            totalDuration += duration_cast<microseconds>(end - start).count();
        }

        double average = totalDuration / 2000.0;
        cout << "Average Case 平均耗時: " << average << " 微秒\n";

        delete[] arr;
        delete[] orig;
        printMemoryUsage();
    }
    else if (mode == 2) {
        // Worst Case: 生成反序資料一次排序
        int* arr = new int[n + 1];
        for (int i = 1; i <= n; ++i) {
            arr[i] = n - i + 1; // n, n-1, ..., 1
        }

        printMemoryUsage();

        auto start = steady_clock::now();
        InsertionSort(arr, n);
        auto end = steady_clock::now();
        long long duration = duration_cast<microseconds>(end - start).count();
        cout << "Worst Case 耗時: " << duration << " 微秒\n";

        delete[] arr;
        printMemoryUsage();
    }
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
