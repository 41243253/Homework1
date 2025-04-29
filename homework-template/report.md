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
1. 時間複雜度：程式的時間複雜度為 $O(n²)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花 $O(n)$次。  
   - 詳細分析: $T(n)$ = $T​(n−1)$ + α/2*​n ⇒ $O(n²)$  
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。  
   - 詳細分析: $S(n)$= $O(n)$  
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n²)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花 $O(n)$次。  
   -詳細分析: $Tw​(n)$= $Tw​(n−1)$+αn ⇒ $O(n²)$ 
2. 空間複雜度：空間複雜度為 $O(n)$，因為在程式中動態分佈了兩個長度為n+1陣列，且經過記憶體量測與計算後確實為n的記憶體花費。  
   - 詳細分析: $S(n)$= $O(n)$  

## 測試與驗證

### 執行結果示意
![Insertion_average](<https://github.com/41243240/Example/blob/main/Insertion_average.png> "Insertion_average")  
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
1. 時間複雜度：程式的時間複雜度為 $O(n log n)$ ，每次都需將新元素插入到已排序的陣列中，比對也要花 $O(n)$次。
    - 詳細分析: $Ta​(n)$= $2Ta​(n/2​)$+ $αn$ ⇒ $ O(𝑛log⁡𝑛)$  
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了三個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
    - 詳細分析: $S(n)$= $O(n)$  
### Worst case:
1. 時間複雜度：程式的時間複雜度為 $O(n²)$ ，對"已排序"或由小到大的排序數據，每次都從最左選一個元素。  
    - 詳細分析: $Tw​(n)$= $Tw​(n−1)$+ $Tw​(0)$+ $αn$ ⇒ $O(n²)$。  
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了三個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
    - 詳細分析: $S(n)$= $O(n)$。  

## 測試與驗證
### 執行結果示意
![Quick_worst](<https://github.com/41243240/Example/blob/main/Quick_worst.png> "Quick_worst")  
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
4.並確認此次的耗費時間是否超過紀錄的時間，若有則輸出時間與資料至max_data.txt，若無則不更新  
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
1. 時間複雜度：程式的時間複雜度為 $O(n log n)$ ，每次排序都是標準的 Merge Sort，對長度 n 的陣列做 ≈ $2T(n/2)$+ $O(n)$ 工作，2000 次循環只是常數倍，不改變漸進量級。
    - 詳細分析: $T(n)$= $2T(n/2​)$+ $αn$⇒ $O(𝑛log⁡𝑛)$
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了三個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
    - 詳細分析: $S(n)$= $O(n)$。 
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n log n)$ ，Merge Sort 無論輸入為何（有序、逆序或隨機），都要完整地對半分+合併，仍是 $O(n log n)$。
    - 詳細分析: $T(n)$= $2T(n/2​)$+ $αn$⇒ $O(𝑛log⁡𝑛)$
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了三個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
    - 詳細分析: $S(n)$= $O(n)$。 

## 測試與驗證
### 執行結果示意
![Merge_average](<https://github.com/41243240/Example/blob/main/Merge_average.png> "Merge_average")  
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

## 程式實作

以下為使用的標頭：

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;
```
以下為使用的函式：
```cpp
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
```
以下為主程式的片段程式碼:
這個片段主要為先初始化亂數，輸入測資數量和選擇需使用排序的case
```cpp
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資：";
    cin >> n;

    int choice;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> choice;
```
若選擇Average case則會讀取檔案的資料(正整數)，並使用同一筆資料執行2000次的循環後將執行時間平均後顯示
```cpp
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
```
### 若選擇Worst case有以下的步驟  
1.則根據輸入的測資數分別做1000、500或10次的循環  
2.並產生n筆資料後呼叫函式打亂  
3.將資料給Heap sort來去做排序
4.並將在循環次數中的耗費最久時間的情況記錄至worst_case_data.txt中
```cpp
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
```
若case選擇錯誤則輸出提示字元並結束程式
```cpp
    else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
```
## 效能分析
### Average case:
1. 時間複雜度：程式的時間複雜度為 $O(n log n)$ ，從文件讀入固定n個資料並對此做排序，每次都是建置好後交換n次，故時間複雜度為 $O(nlogn)$。
   - 詳細分析: $Tw​(n)$= $Tw​(n−1)$+ $βlogn$ ⇒ $𝑇(𝑛)$= $𝛼𝑛$+ $∑𝛽log⁡𝑘$= $O(𝑛log⁡𝑛)$。  
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了兩個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
   - 詳細分析: $S(n)$= $O(n)$。 
### Worst case:
1. 時間複雜度：程式的時間複雜度也為 $O(n log n)$ ，產生固定n筆資料並做排序，也是根據建置的資料來去交換n次，個時間複雜度為 $O(nlogn)$。
   - 詳細分析: $Tw​(n)$= $Tw​(n−1)$+ $βlogn$ ⇒ $𝑇(𝑛)$= $𝛼𝑛$+ $∑𝛽log⁡𝑘$= $O(𝑛log⁡𝑛)$。   
2. 空間複雜度：空間複雜度為 $O(n)$，額外分配了三個大小≈n的陣列，合併時需要 $O(n)$的空間，且經過記憶體量測與計算後確實為n的記憶體花費。
   - 詳細分析: $S(n)$= $O(n)$。 

## 測試與驗證
### 執行結果示意
![Heap_worst](<https://github.com/41243240/Example/blob/main/Heap_worst.png> "Heap_worst")  
### 測試案例

|  測資數量   | Worst case(microseconds)   | Average case(microseconds) | 
|------------|----------------------------|----------------------------|
| $n=500$    |     433     | 59.813        |
| $n=1000$   |     1306      | 133.557        |
| $n=2000$   |     1755      | 291.417       |
| $n=3000$   |     1877      | 457.263      |
| $n=4000$   |     2213     | 634.300 | 
| $n=5000$   |     2447     | 814.952 |

![Heap](<https://github.com/41243240/Example/blob/main/Heap.png> "Heap")  
雖然Average case和Worst case理應為差不多的曲線，但因測試時是取最糟糕的資料與時間，其中包含了程式的預熱這些的因素，所以結果才會有明顯的差異，但在實際執行與查看時大部分時間是與Average case差不多的。
### Composite sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生對應case的耗費時間  
3. 根據筆數去選擇最短時間的排序法
4. 排序後輸出花費的時間與記憶體花費

## 程式實作

以下為使用的標頭：

```cpp
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
```
以下為使用的函式：
```cpp
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
```
以下為主程式的程式碼:
這邊的片段主要為輸入測資數量和選擇需使用排序的case
```cpp
int main() {
    srand((unsigned)time(nullptr));
    printMemoryUsage();

    int n;
    cout << "請輸入要幾筆測資: ";
    cin >> n; 

    int mode;
    cout << "選擇模式 (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;
```
若選擇Average case則會讀取檔案的資料(正整數)，之後跑Merge sort，並執行2000次的循環後將執行時間平均後顯示
```cpp
if (mode == 1) {
        // 平均情況
        int* orig = new int[n + 1];
        ifstream fin("testdata500.txt");
        if (!fin) {
            cerr << "無法開啟 testdata500.txt\n";
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
```
若選擇Worst case則會根據一開始輸入的n來做判斷，如果n在500、1000、2000時，會執行Quick sort，如果n在3000、4000、5000時，則會執行Heap sort
```cpp
else if (mode == 2) {
        // 最壞情況
        int* arr = new int[n + 1];
        int* tmp = new int[n + 1];
        printMemoryUsage();

        long long duration;
        string selectedAlgorithm;

        if (n == 500 || n == 1000 || n == 2000) {
            selectedAlgorithm = "Quick Sort";
            for (int i = 1; i <= n; ++i) arr[i] = i;
            auto start = steady_clock::now();
            QuickSortIterative(arr, 1, n);
            auto end = steady_clock::now();
            duration = duration_cast<microseconds>(end - start).count();
        }
        else if (n == 3000 || n == 4000 || n == 5000) {
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
```
若在選擇case時輸入錯誤則輸出提示字元後結束程式
```cpp
else {
        cout << "選項錯誤，程式結束。\n";
        return 1;
    }

    return 0;
}
```
## 效能分析
### Average case:
1.時間複雜度：

2.空間複雜度：
### Worst case:
Quick sort
1.時間複雜度：

2.空間複雜度：


Heap sort
1.時間複雜度：

2.空間複雜度：
## 測試與驗證

### 執行結果示意
平均情況(Merge sort)

![AverageCase_Merge](<https://github.com/41243253/example-1/blob/main/AverageCaseMerge.png> "AverageCase_Merge")

最壞情況(Quick sort)

![WorstCase_Quick](<https://github.com/41243253/example-1/blob/main/WorstCaseQuick.png> "WorstCase_Quick")

最壞情況(Heap sort)

![WorstCase_Heap](<https://github.com/41243253/example-1/blob/main/WorstCaseHeap.png> "WorstCase_Heap")

### 測試案例

經過前面四個排序法的測式統整，在最壞情況下，n=500、1000、2000時是Quick sort最快，因此使用Quick sort，而n=3000、4000、5000時是Heap sort最快，因此使用Heap sort，在平均情況下在任何的測資數中都是使用Merge sort

|  測資數量   | Worst case(microseconds)   | Average case(microseconds) | 
|------------|----------------------------|----------------------------|
| $n=500$    |     185 (Quick sort)     | 31.439 (Merge sort)        |
| $n=1000$   |     529 (Quick sort)      | 78.772 (Merge sort)        |
| $n=2000$   |     1703 (Quick sort)      | 183.914 (Merge sort)       |
| $n=3000$   |     623 (Heap sort)      | 291.267 (Merge sort)      |
| $n=4000$   |     919 (Heap sort)     | 400.697 (Merge sort) | 
| $n=5000$   |     1076 (Heap sort)     | 512.725 (Merge sort) |

## 結論
### 各排序方法的Worst Case耗費時間比較
![worst_case](<https://github.com/41243240/Example/blob/main/worst_case.png> "worst case")
### 各排序方法的Average Case耗費時間比較
![average](<https://github.com/41243240/Example/blob/main/average.png> "average")  

### 各排序法複雜度一覽

  | 演算法            | 平均時間     | 最壞時間    | 空間    |
  |------------------|------------|------------|-------|
  | Insertion Sort   | $O(n²)$ | $O(n²)$ | $O(n)$ |
  | Quick Sort       | $O(nlogn)$ | $O(n²)$ | $O(n)$ |
  | Merge Sort       | $O(nlogn)$ | $O(nlogn)$ | $O(n)$ |
  | Heap Sort        | $O(nlogn)$ | $O(nlogn)$ | $O(n)$ |  
  
1. 各個排序程式能正確從小到大做排序。  
2. 在選擇不同的Case能產出對應的花費時間。  
3. 測試案例涵蓋了多筆測資來測試其時間複雜度是否符合（ $n = 500$、 $n = 1000$、 $n = 2000$、 $n = 3000$、 $n = 4000$、 $n = 5000$ ），並驗證程式的正確性。  
## 申論及開發報告

- **設計動機**  
  比較 Insertion/Quick/Merge/Heap 四種排序在平均及最壞情況下的效能差異，驗證理論複雜度。

- **實作重點**  
  - **Insertion Sort**：
    - 實作方式：利用哨兵位元 (a[0]) 簡化邊界檢查，Insert 函式內透過迴圈搬移元素；以 memcpy 還原原始資料保證每輪測試初始狀態一致。  
    - 選擇理由：小規模資料下插入排序最直觀且效率不差；哨兵化與 memcpy 設計讓批量測試流程更簡潔公平。  
  - **Quick Sort**：  
    - 實作方式：最左元素當 pivot，非遞迴用顯式 stack[] 模擬呼叫堆疊，並優先處理大區間以降低最大堆疊深度。  
    - 選擇理由：左樞紐最易理解，顯式堆疊與大小區間策略防止深度溢出；可清楚展示隨機資料下 $O(nlogn)$與最壞 $O(n²)$差異。  
  - **Merge Sort**：  
    - 實作方式：遞迴分治 + 線性合併(merge)，合併時全部複製到輔助陣列 tmp；同樣以 memcpy 還原確保一致測試。   
    - 選擇理由：遞迴寫法最貼近理論描述，無論輸入皆 $O(nlogn)$；固定大小的 tmp 陣列使合併流程清晰且易於效能量測。  
  - **Heap Sort**：  
    - 實作方式：先做一次自底向上建堆 $O(n)$，再反覆 Extract-Max $O(nlog n)$，Average case 以 memcpy 還原隨機資料，Worst case 多次打亂並挑最慢樣本記錄其原始排列。  
    - 選擇理由：  $O(1)$核心空間、典型 $O(nlogn)$演算法；Worst case 樣本捕捉設計凸顯隨機與最差情境性能差異。  

- **測試策略**  
  - **Average Case**：同一筆隨機資料，多次 `memcpy` 還原，2000 次排序取平均。  
  - **Worst Case**：  
    - Insertion/Quick：直接產生反序或有序陣列，單次計時。  
    - Merge/Heap：多次隨機打亂，挑最慢一次並記錄。

- **結論**  
  - 小規模資料選 Insertion。  
  - Quick Sort 需避開極端有序輸入或改用三數取中；  
  - 大型資料建議 Merge 或 Heap，維持穩定 $O(nlogn)$ 性能。
  - 本次實驗亦強調了「測試設計」的重要性：如何公平地比較不同演算法的效能，並正確解讀系統性雜訊。
