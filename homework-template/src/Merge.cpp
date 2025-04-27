#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// 顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS m;
    GetProcessMemoryInfo(GetCurrentProcess(), &m, sizeof(m));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << m.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

// 隨機打亂 arr[1..n]
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

// 合併 [l..m] 和 [m+1..r]
void merge(int* a, int l, int m, int r, int* tmp) {
    for (int i = l; i <= r; ++i) tmp[i] = a[i];
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (tmp[i] <= tmp[j]) a[k++] = tmp[i++];
        else                  a[k++] = tmp[j++];
    }
    while (i <= m) a[k++] = tmp[i++];
    while (j <= r) a[k++] = tmp[j++];
}

// 遞迴 Merge Sort
void mergeSort(int* a, int l, int r, int* tmp) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m, tmp);
    mergeSort(a, m + 1, r, tmp);
    merge(a, l, m, r, tmp);
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
        // Average Case：讀檔案測2000次
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
        int* tmp = new int[n + 1];
        printMemoryUsage();

        long long totalDuration = 0;
        for (int k = 0; k < 2000; ++k) {
            memcpy(arr + 1, orig + 1, n * sizeof(int));

            auto start = steady_clock::now();
            mergeSort(arr, 1, n, tmp);
            auto end = steady_clock::now();
            totalDuration += duration_cast<microseconds>(end - start).count();
        }

        double average = totalDuration / 2000.0;
        cout << "Average Case 平均耗時: " << average << " 微秒\n";

        delete[] arr;
        delete[] orig;
        delete[] tmp;
        printMemoryUsage();
    }
    else if (mode == 2) {
        // Worst Case：保留你原本生成亂數的做法
        int loopcount;
        cout << "循環幾次: ";
        cin >> loopcount;

        printMemoryUsage();
        while (loopcount > 0) {
            loopcount -= 1;

            // 建立 1..n 並打亂
            int* arr = new int[n + 1];
            for (int i = 1; i <= n; ++i) arr[i] = i;
            permute(arr, n);
            cout << "已生成並隨機打亂 " << n << " 筆測資。\n";

            int* orig = new int[n + 1];
            memcpy(orig + 1, arr + 1, n * sizeof(int));

            int* tmp = new int[n + 1];

            // Merge Sort 計時
            auto start = steady_clock::now();
            mergeSort(arr, 1, n, tmp);
            auto end = steady_clock::now();
            long long dur = duration_cast<microseconds>(end - start).count();
            cout << "Merge Sort 耗時: " << dur << " 微秒\n";

            // 更新 max_data.txt
            const char* F = "max_data.txt";
            long long prev = 0;
            ifstream infile(F);
            if (infile >> prev) infile.close();

            if (dur > prev) {
                ofstream outfile(F, ios::trunc);
                outfile << dur << "\n";
                for (int i = 1; i <= n; ++i) {
                    outfile << orig[i] << (i < n ? ' ' : '\n');
                }
                cout << "已更新 " << F << "（含原始未排序測資）。\n";
            }
            else {
                cout << "目前最大耗時 " << prev << " 微秒，未更新檔案。\n";
            }

            delete[] arr;
            delete[] orig;
            delete[] tmp;
        }
        printMemoryUsage();
    }
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
