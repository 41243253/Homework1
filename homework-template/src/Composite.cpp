#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>    // 記憶體操作（像是memcpy）
#include <chrono>     // 時間量測工具
#include <Windows.h>  // Windows API（用來取得記憶體資訊）
#include <Psapi.h>    // 輔助記憶體資訊用
using namespace std;
using namespace chrono;

// 顯示目前程式使用的記憶體大小（Working Set Size）
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n";
    cout << "Memory Usage Information:\n";
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n"; // 單位轉成KB
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

// 合併排序（遞迴版）
void merge(int* a, int l, int m, int r, int* tmp) {
    // 把目前區間[l..r]暫存到tmp陣列中
    for (int i = l; i <= r; ++i) tmp[i] = a[i];

    int i = l, j = m + 1, k = l;
    // 合併左邊[l..m]和右邊[m+1..r]，兩邊都是已排序
    while (i <= m && j <= r) {
        if (tmp[i] <= tmp[j]) a[k++] = tmp[i++]; // 小的先放進主陣列
        else a[k++] = tmp[j++];
    }
    // 把左邊或右邊剩下的資料直接接上去
    while (i <= m) a[k++] = tmp[i++];
    while (j <= r) a[k++] = tmp[j++];
}

void mergeSort(int* a, int l, int r, int* tmp) {
    if (l >= r) return; // 區間只有一個元素或無元素時結束
    int m = (l + r) / 2; // 找中間
    mergeSort(a, l, m, tmp);   // 左邊遞迴
    mergeSort(a, m + 1, r, tmp); // 右邊遞迴
    merge(a, l, m, r, tmp);   // 合併左右區間
}

// 堆積排序
template <class T>
void Adjust(T* a, const int root, const int n) {
    T e = a[root]; // 暫存root
    int j;
    for (j = 2 * root; j <= n; j *= 2) { // 往下找
        if (j < n && a[j] < a[j + 1]) j++; // 找左右子中較大的
        if (e >= a[j]) break; // 如果e已經比子節點大，停止
        a[j / 2] = a[j]; // 上移子節點
    }
    a[j / 2] = e; // 放入正確位置
}

template <class T>
void HeapSort(T* a, const int n) {
    // 堆積
    for (int i = n / 2; i >= 1; i--) {
        Adjust(a, i, n);
    }
    // 不斷取出最大值，並調整剩下的堆積
    for (int i = n - 1; i >= 1; i--) {
        swap(a[1], a[i + 1]); // 最大的元素移到最後
        Adjust(a, 1, i); // 調整堆積
    }
}

// 主程式
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
        // 平均情況
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
        int* tmp = new int[n + 1];
        printMemoryUsage();

        long long totalDuration = 0;
        string selectedAlgorithm;

        if (n == 500 || n == 1000 || n == 2000 || n == 3000 || n == 4000 || n == 5000) {
            selectedAlgorithm = "Merge Sort";
            for (int k = 0; k < 2000; ++k) {
                memcpy(arr + 1, orig + 1, n * sizeof(int)); // 複製原始資料到arr
                auto start = steady_clock::now();
                mergeSort(arr, 1, n, tmp);
                auto end = steady_clock::now();
                totalDuration += duration_cast<microseconds>(end - start).count(); 
            }
        }
        else {
            cout << "此 n 沒有對應設定，預設使用 Merge Sort。\n";
            selectedAlgorithm = "Merge Sort";
            for (int k = 0; k < 2000; ++k) {
                memcpy(arr + 1, orig + 1, n * sizeof(int));
                auto start = steady_clock::now();
                mergeSort(arr, 1, n, tmp);
                auto end = steady_clock::now();
                totalDuration += duration_cast<microseconds>(end - start).count();
            }
        }

        double average = totalDuration / 2000.0;
        cout << "使用的排序法是：" << selectedAlgorithm << endl;
        cout << "Average Case 平均耗時: " << average << " 微秒\n";

        delete[] arr;
        delete[] orig;
        delete[] tmp;
        printMemoryUsage(); // 排序後再看一次記憶體
    }
    else if (mode == 2) {
        // 最壞情況
        int* arr = new int[n + 1];
        int* tmp = new int[n + 1];
        printMemoryUsage();

        long long duration;
        string selectedAlgorithm;

        if (n == 500 || n == 1000 ) {
            selectedAlgorithm = "Quick Sort";
            for (int i = 1; i <= n; ++i) arr[i] = i;
            auto start = steady_clock::now();
            QuickSortIterative(arr, 1, n);
            auto end = steady_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
        }
        else if (n==2000 || n == 3000 || n == 4000 || n == 5000) {
            selectedAlgorithm = "Heap Sort";
            for (int i = 1; i <= n; ++i) arr[i] = i;
            auto start = steady_clock::now();
            HeapSort(arr, n);
            auto end = steady_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
        }
        else {
            cout << "此 n 沒有對應設定，預設使用 Heap Sort。\n";
            selectedAlgorithm = "Heap Sort";
            for (int i = 1; i <= n; ++i) arr[i] = i;
            auto start = steady_clock::now();
            HeapSort(arr, n);
            auto end = steady_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
        }

        cout << "使用的排序法是：" << selectedAlgorithm << endl;
        cout << "Worst Case 耗時: " << duration << " 微秒\n";

        delete[] arr;
        delete[] tmp;
        printMemoryUsage();
    }
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
