#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// 顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------" << endl;
    cout << "Memory Usage Information:" << endl;
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout << "----------------------------------------------------------" << endl;
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

// 產生固定升序資料（1 到 n），arr[1] ~ arr[n] 使用
int* generateFixedData(int n) {
    int* arr = new int[n + 2];
    for (int i = 1; i <= n; ++i) {
        arr[i] = i;
    }
    return arr;
}

// 將 arr[1..n] 隨機打亂
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

int main() {
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇測資模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;

    // 建立資料
    int* arr = generateFixedData(n);
    if (mode == 1) {
        permute(arr, n);
        cout << "使用 Average Case 測資 (隨機打亂)" << endl;
    }
    else if (mode == 2) {
        cout << "使用 Worst Case 測資 (升序排列)" << endl;
    }
    else {
        cout << "選項錯誤，程式結束。" << endl;
        delete[] arr;
        return 0;
    }

    printMemoryUsage();

    // 計時並排序
    auto start = steady_clock::now();
    QuickSortIterative(arr, 1, n);
    auto end = steady_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    cout << "耗時：" << duration.count() << " 微秒" << endl;

    delete[] arr;
    printMemoryUsage();
    return 0;
}
