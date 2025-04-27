#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>   // for memcpy
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// 顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n";
    cout << "Memory Usage Information:\n";
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n";
    cout << "----------------------------------------------------------\n";
}

// 非遞迴 QuickSort（pivot 固定為最左邊）
template <class T>
void QuickSortIterative(T* a, int left, int right) {
    struct Range { int l, r; };
    int maxSize = right - left + 1;
    Range* stack = new Range[maxSize];
    int top = 0;
    stack[top] = { left, right };

    while (top >= 0) {
        Range cur = stack[top--];
        int L = cur.l, R = cur.r;
        if (L < R) {
            // partition
            T pivot = a[L];
            int i = L + 1, j = R;
            while (true) {
                while (i <= R && a[i] < pivot) ++i;
                while (j >= L + 1 && a[j] > pivot) --j;
                if (i < j) swap(a[i], a[j]);
                else break;
            }
            swap(a[L], a[j]);  // pivot 放中間

            // 先壓較大區間，後壓較小區間
            int leftSize = j - 1 - L;
            int rightSize = R - (j + 1);
            if (leftSize > rightSize) {
                if (L < j - 1)     stack[++top] = { L,     j - 1 };
                if (j + 1 < R)     stack[++top] = { j + 1, R };
            }
            else {
                if (j + 1 < R)     stack[++top] = { j + 1, R };
                if (L < j - 1)     stack[++top] = { L,     j - 1 };
            }
        }
    }

    delete[] stack;
}

int main() {
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;

    if (mode == 1) {
        // Average Case: 從檔案讀資料，重複排序2000次
        int* orig = new int[n + 1];
        ifstream fin("testdata5000.txt");
        if (!fin) {
            cerr << "無法開啟 testdata5000.txt\n";
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
            QuickSortIterative(arr, 1, n);
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
        // Worst Case: 生成升序資料
        int* arr = new int[n + 1];
        for (int i = 1; i <= n; ++i) {
            arr[i] = i; // 1, 2, ..., n
        }

        printMemoryUsage();

        auto start = steady_clock::now();
        QuickSortIterative(arr, 1, n);
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
