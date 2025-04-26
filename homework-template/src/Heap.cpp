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
    T e = a[root];
    int j;
    for (j = 2 * root; j <= n; j *= 2)
    {
        if (j < n && a[j] < a[j + 1]) j++;
        if (e >= a[j]) break;
        a[j / 2] = a[j];
    }
    a[j / 2] = e;
}


// 堆積排序
template <class T>
void HeapSort(T* a, const int n)
{
    // 建立堆積
    for (int i = n / 2; i >= 1; i--)
    {
        Adjust(a, i, n);
    }

    for (int i = n - 1; i >= 1; i--) {
        swap(a[1], a[i + 1]);
        Adjust(a, 1, i);
    }
}

int main()
{
    srand((unsigned)time(nullptr)); // 隨機種子
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資：";
    cin >> n;
    int choice;
    cout << "選擇版本 1.Average case, 2.Worst case: ";
    cin >> choice;
    // 動態配置陣列，arr[1..n]
    int* arr = new int[n + 1];
    int* tempArr = new int[n + 1];
    int* worstCaseArr = new int[n + 1];

    printMemoryUsage();

    int testCount;
    // 測試次數：小n多次，大n少次
    if (n <= 1000) testCount = 1000;
    else if (n <= 10000) testCount = 500;
    else testCount = 10;

    long long worst_time = -1; // 最慢耗時（單位：微秒）
    long long average_time = 0; // 平均耗時(單位：微秒)
    // 開始測試
    for (int t = 0; t < testCount; ++t)
    {
        // 初始化 arr 為 1..n
        for (int i = 1; i <= n; ++i) {
            arr[i] = i;
        }
        permute(arr, n); // 隨機打亂

        // 把目前排列備份到 tempArr（備份原始排列）
        for (int i = 1; i <= n; ++i) {
            tempArr[i] = arr[i];
        }

        // 開始計時
        auto start = steady_clock::now();
        HeapSort(arr, n);
        auto end = steady_clock::now();

        // 計算耗時（微秒）
        auto duration = duration_cast<microseconds>(end - start).count();
        if (choice == 1) {
            average_time += duration;
        }
        // 如果這次比前面慢，更新最差情況
        if (duration > worst_time)
        {
            worst_time = duration;
            // 把 tempArr（原排列）存進 worstCaseArr
            for (int i = 1; i <= n; ++i) {
                worstCaseArr[i] = tempArr[i];
            }
        }
    }
    if (choice == 1) {
        average_time /= testCount;
        cout << "平均耗時：" << average_time << " 微秒" << endl;
    }
    else {
    cout << "最差情況耗時：" << worst_time << " 微秒" << endl;
    }
    // 將最差情況的排列存成文字檔
    ofstream outFile("worst_case_data.txt");
    if (outFile.is_open())
    {
        for (int i = 1; i <= n; ++i)
        {
            outFile << worstCaseArr[i] << " ";
        }
        outFile.close();
        cout << "最慢的排列資料已經儲存到 'worst_case_data.txt'" << endl;
    }
    else
    {
        cerr << "錯誤：無法寫入檔案！" << endl;
    }

    delete[] arr;
    delete[] tempArr;
    delete[] worstCaseArr;

    printMemoryUsage();
    return 0;
}
