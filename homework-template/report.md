# 41243240
# 41243253

作業一

## 解題說明

本題要求實現Insertion sort,Quick sort,Merge sort和Heap sort的
Worst case和Average case所耗費的時間，並計算其空間複雜度

### Insertion sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生或讀入資料
3. 將測試資料插入陣列中並做排序
4. 排序後輸出花費的時間與記憶體花費

## 程式實作

以下為使用的標頭：

```cpp
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstring>   // for memcpy
#include <chrono>    // 時間測量功能
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono; // 方便使用 steady_clock
```
以下為使用的函式：
```cpp
//插入元素到已經排序好的串列中
template <class T>
void Insert(const T& e, T* a, int i)
{
    a[0] = e;
    while (e < a[i])
    {
        a[i + 1] = a[i];
        i--;
    }
    a[i + 1] = e;
}

//插入排序法
template <class T>
void InsertionSort(T* a, const int n)
{
    for (int j = 2; j <= n; j++)
    {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

//顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout<<"----------------------------------------------------------"<<endl;
    cout<<"Memory Usage Information:" << endl;
    cout<<"Working Set Size: "<<memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout<<"----------------------------------------------------------"<<endl;
}
```
以下為主程式的程式碼:  
這邊的片段主要為輸入測資數量和選擇需使用排序的case
```cpp
int main() {
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;
```
若選擇Average case則會讀取檔案的資料(正整數)，並執行2000次的循環後將執行時間平均後顯示
```cpp
    if (mode == 1) {
        // Average Case: 從 data.txt 讀入 n 筆資料，重複排序 2000 次取平均
        int* orig = new int[n + 1];
        ifstream fin("data.txt");
        if (!fin) {
            cerr << "無法開啟 data.txt\n";
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
```
若選擇Worst case則會根據一開始輸入的n來產生n~1的資料來去做排序，以此來產生Worst case的情況
```cpp
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
```
## 效能分析
### Average case:
1. 時間複雜度：程式的時間複雜度為 $O(n²)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n²)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。

## 測試與驗證

### 測試案例

|  測資數量   | Worst case(microseconds)   | Average case(microseconds) | 
|------------|----------------------------|----------------------------|
| $n=500$    |     222      | 94.861        |
| $n=1000$   |     693      | 315.059        |
| $n=2000$   |     2828      | 1268.36       |
| $n=3000$   |     4658      | 2572.673       |
| $n=4000$   |     8503     | 4657.093 | 
| $n=5000$   |     12506     | 9541.387 |

![Insertion](<https://github.com/41243240/Example/blob/main/Insertion.png> "Insertion")


### 編譯與執行指令

```shell
g++ -std=c++17 -O2 Insertion.cpp -o Insertion
./Insertion
```
### Quick sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生對應case的耗費時間  
3. 讀入資料後根據排序邏輯去排序
4. 排序後輸出花費的時間與記憶體花費

## 程式實作

以下為使用的標頭：

```cpp
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
```
以下為使用的函式：
```cpp
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
// 顯示記憶體使用程度
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------\n";
    cout << "Memory Usage Information:\n";
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB\n";
    cout << "----------------------------------------------------------\n";
}
```
以下為主程式的片段程式碼:  
這個片段主要為先初始化亂數，輸入測資數量和選擇需使用排序的case
```cpp
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;
```
若選擇Average case則會讀取檔案的資料(正整數)，並使用同一筆資料執行2000次的循環後將執行時間平均後顯示
```cpp
   if (mode == 1) {
        // Average Case: 從檔案讀資料，重複排序2000次
        int* orig = new int[n + 1];
        ifstream fin("data.txt");
        if (!fin) {
            cerr << "無法開啟 data.txt\n";
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
```
若選擇Worst case則會根據一開始輸入的n來產生1~n的資料來去做排序，以此來產生Worst case的情況，若選擇的模式錯誤則輸出提示字元並結束程式
```cpp
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
```
## 效能分析
### Average case:
1. 時間複雜度：程式的時間複雜度為 $O(n log n)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n²)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。

## 測試與驗證

### 測試案例

|  測資數量   | Worst case(microseconds)   | Average case(microseconds) | 
|------------|----------------------------|----------------------------|
| $n=500$    |     133      | 85.692        |
| $n=1000$   |     358      | 196.415        |
| $n=2000$   |     1332      | 437.87       |
| $n=3000$   |     2853      | 698.887       |
| $n=4000$   |     4994     | 986.374 | 
| $n=5000$   |     8502     | 1261.150 |

![Quick](<https://github.com/41243240/Example/blob/main/Quick.png> "Quick")
### Merge sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生對應case的耗費時間  
3. 讀入資料後根據排序邏輯去排序
4. 排序後輸出花費的時間與記憶體花費
## 程式實作

以下為使用的標頭：

```cpp
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
```
以下為使用的函式：  
此片段為資料產生的方式，在經過亂數產生後將資料內容給隨機打亂
```cpp
// 隨機打亂 arr[1..n]
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

```
此片段為將兩筆不同的資料合併在一起
```cpp
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
```
此片段為Merge Sort使用遞迴來去撰寫，除了應用到他的排序邏輯在使用遞迴也能讓程式更加簡潔
```cpp
// 遞迴 Merge Sort
void mergeSort(int* a, int l, int r, int* tmp) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m, tmp);
    mergeSort(a, m + 1, r, tmp);
    merge(a, l, m, r, tmp);
}
```
以下為主程式的片段程式碼:  
這個片段主要為先初始化亂數，輸入測資數量和選擇需使用排序的case
```cpp
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n;

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;
```
若選擇Average case則會讀取檔案的資料(正整數)，並使用同一筆資料執行2000次的循環後將執行時間平均後顯示
```cpp
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
```
### 若選擇Worst case則會有以下幾步驟  
1.輸入要循環幾次後執行  
2.根據一開始輸入的n來產生n筆亂數資料之後呼叫函式打亂資料  
3.將產生的資料丟給Merge Sort做排序  
4.並確認此次的耗費時間是否超過紀錄的時間，若有則輸出時間與資料至.txt，若無則不更新  
```cpp
    else if (mode == 2) {
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
```
若在選擇case時輸入錯誤則輸出提示字元後結束程式
```cpp
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
```
## 效能分析
### Average case:
1. 時間複雜度：程式的時間複雜度為 $O(n log n)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n log n)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花$O(n)$次。
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。

## 測試與驗證

### 測試案例

|  測資數量   | Worst case(microseconds)   | Average case(microseconds) | 
|------------|----------------------------|----------------------------|
| $n=500$    |     555      | 31.750        |
| $n=1000$   |     1260      | 76.029        |
| $n=2000$   |     1743      | 180.106       |
| $n=3000$   |     2345      | 282.655      |
| $n=4000$   |     2632     | 392.676 | 
| $n=5000$   |     2456     | 499.824 |

![Merge](<https://github.com/41243240/Example/blob/main/Merge.png> "Merge")  
雖然Average case和Worst case理應為差不多的曲線，但因測試時是取最糟糕的資料與時間，其中包含了程式的預熱這些的因素，所以結果才會有明顯的差異，但在實際執行與查看時大部分時間是與Average case差不多的。
### Heap sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生對應case的耗費時間  
3. 將測試資料插入陣列中並做排序
4. 排序後輸出花費的時間與記憶體花費
### 結論

![worst_case](<https://github.com/41243240/Example/blob/main/worst_case.png> "worst case")


1. 程式能正確計算 $n$ 到 $1$ 的連加總和。  
2. 在 $n < 0$ 的情況下，程式會成功拋出異常，符合設計預期。  
3. 測試案例涵蓋了多種邊界情況（$n = 0$、$n = 1$、$n > 1$、$n < 0$），驗證程式的正確性。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，使用遞迴來計算連加總和的主要原因如下：

1. **程式邏輯簡單直觀**  
   遞迴的寫法能夠清楚表達「將問題拆解為更小的子問題」的核心概念。  
   例如，計算 $\Sigma(n)$ 的過程可分解為：  

   $$
   \Sigma(n) = n + \Sigma(n-1)
   $$

   當 $n$ 等於 1 或 0 時，直接返回結果，結束遞迴。

2. **易於理解與實現**  
   遞迴的程式碼更接近數學公式的表示方式，特別適合新手學習遞迴的基本概念。  
   以本程式為例：  

   ```cpp
   int sigma(int n) {
       if (n < 0)
           throw "n < 0";
       else if (n <= 1)
           return n;
       return n + sigma(n - 1);
   }
   ```

3. **遞迴的語意清楚**  
   在程式中，每次遞迴呼叫都代表一個「子問題的解」，而最終遞迴的返回結果會逐層相加，完成整體問題的求解。  
   這種設計簡化了邏輯，不需要額外變數來維護中間狀態。

透過遞迴實作 Sigma 計算，程式邏輯簡單且易於理解，特別適合展示遞迴的核心思想。然而，遞迴會因堆疊深度受到限制，當 $n$ 值過大時，應考慮使用迭代版本來避免 Stack Overflow 問題。
