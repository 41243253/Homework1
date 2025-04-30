#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// 將 arr[1..n] 隨機打亂
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

// 顯示目前程式的記憶體使用情況
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------" << endl;
    cout << "Memory Usage Information:" << endl;
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout << "----------------------------------------------------------" << endl;
}

// 調整最大堆積
template <class T>
void Adjust(T* a, const int root, const int n)
{
    T e = a[root]; // 暫存root
    int j;
    for (j = 2 * root; j <= n; j *= 2)
    { // 往下找
        if (j < n && a[j] < a[j + 1]) j++; // 找左右子中較大的
        if (e >= a[j]) break; // 如果e已經比子節點大，停止
        a[j / 2] = a[j]; // 上移子節點
    }
    a[j / 2] = e; // 放入正確位置
}

// 堆積排序
template <class T>
void HeapSort(T* a, const int n)
{
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

int main() {
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資：";
    cin >> n;

    int choice;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> choice;

    if (choice == 1) {
        // Average Case
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
            memcpy(arr + 1, orig + 1, n * sizeof(int));
            auto start = steady_clock::now();
            HeapSort(arr, n);
            auto end = steady_clock::now();
            totalDuration += duration_cast<microseconds>(end - start).count();
        }

        double average = totalDuration / 2000.0;
        cout << "Average Case 平均耗時: " << average << " 微秒\n";

        delete[] arr;
        delete[] orig;
        printMemoryUsage();
    }
    else if (choice == 2) {
        // Worst Case
        int* arr = new int[n + 1];
        int* tempArr = new int[n + 1];
        int* worstCaseArr = new int[n + 1];

        printMemoryUsage();

        int testCount;
        if (n <= 1000) testCount = 1000;
        else if (n <= 10000) testCount = 500;
        else testCount = 10;

        long long worst_time = -1;

        for (int t = 0; t < testCount; ++t) {
            for (int i = 1; i <= n; ++i) {
                arr[i] = i;
            }
            permute(arr, n);

            for (int i = 1; i <= n; ++i) {
                tempArr[i] = arr[i];
            }

            auto start = steady_clock::now();
            HeapSort(arr, n);
            auto end = steady_clock::now();

            auto duration = duration_cast<microseconds>(end - start).count();
            if (duration > worst_time) {
                worst_time = duration;
                for (int i = 1; i <= n; ++i) {
                    worstCaseArr[i] = tempArr[i];
                }
            }
        }

        cout << "最差情況耗時：" << worst_time << " 微秒\n";

        ofstream outFile("worst_case_data.txt");
        if (outFile.is_open()) {
            for (int i = 1; i <= n; ++i) {
                outFile << worstCaseArr[i] << " ";
            }
            outFile.close();
            cout << "最慢的排列資料已經儲存到 'worst_case_data.txt'\n";
        }
        else {
            cerr << "錯誤：無法寫入檔案！\n";
        }

        delete[] arr;
        delete[] tempArr;
        delete[] worstCaseArr;
        printMemoryUsage();
    }
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
