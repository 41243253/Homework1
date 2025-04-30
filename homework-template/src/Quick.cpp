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

// 快速排序(非遞迴版）
template <class T>
void QuickSortIterative(T* a, int left, int right) {
    struct Range { int l, r; }; // 定義一個範圍結構，儲存左右邊界
    int maxSize = right - left + 1; // Stack大小設定為資料數量（最壞情況）
    Range* stack = new Range[maxSize]; // 動態分配stack
    int top = 0; // stack頂端指標
    stack[top] = { left, right }; // 將初始範圍推進stack

    while (top >= 0) { // 當stack還有東西
        Range cur = stack[top--]; // 取出一個區間來處理
        int L = cur.l, R = cur.r;
        if (L < R) {
            T pivot = a[L]; // 取最左邊作為pivot
            int i = L + 1, j = R;
            while (true) {
                while (i <= R && a[i] < pivot) ++i; // 找到>=pivot的
                while (j >= L + 1 && a[j] > pivot) --j; // 找到<=pivot的
                if (i < j) swap(a[i], a[j]); // 交換兩邊錯放的資料
                else break;
            }
            swap(a[L], a[j]); // 把pivot放到正確位置（中間）

            // 為了避免stack過大，先推入較大的那邊（空間小優先）
            int leftSize = j - 1 - L;
            int rightSize = R - (j + 1);
            if (leftSize > rightSize) {
                if (L < j - 1) stack[++top] = { L, j - 1 }; // 左邊有資料就推入
                if (j + 1 < R) stack[++top] = { j + 1, R }; // 右邊有資料就推入
            }
            else {
                if (j + 1 < R) stack[++top] = { j + 1, R };
                if (L < j - 1) stack[++top] = { L, j - 1 };
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
