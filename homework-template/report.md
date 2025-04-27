# 41243240
# 41243253

作業一

## 解題說明

本題要求實現Insertion sort,Quick sort,Merge sort和Heap sort的
Worst case和Average case所耗費的時間，並計算其空間複雜度

### Insertion sort 解題策略

1. 輸入測資數量後產生測試資料
2. 根據選擇的case產生對應case的耗費時間  
3. 將測試資料插入陣列中並做排序
4. 排序後輸出花費的時間

## 程式實作

以下為使用的標頭：

```cpp
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>  // 時間測量功能
#include <Windows.h> 
#include <Psapi.h> //用來量測記憶體
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
```cpp
int main(void)
{
    printMemoryUsage();
    int n;
    cout << "請輸入要幾筆測資:";
    cin >> n;
    int* arr = new int[n + 1]; // arr[0] 是哨兵位，1~n 為資料
    int temp = n;
    // 產生隨機數n~1
    for (int i = 1; i <= n; i++)
    {
        arr[i] = temp;
        temp -= 1;
    }

    printMemoryUsage();
    // 開始計時
    auto start = steady_clock::now();

    InsertionSort(arr, n);

    // 結束計時
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "耗時：" << duration.count() << " 微秒" << endl;

    delete[] arr;
    printMemoryUsage();
    return 0;
}
```
## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(\n²)$。
2. 空間複雜度：空間複雜度為 $O(100\times \log n + \pi)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $n = 0$      | 0        | 0        |
| 測試二   | $n = 1$      | 1        | 1        |
| 測試三   | $n = 3$      | 6        | 6        |
| 測試四   | $n = 5$      | 15       | 15       |
| 測試五   | $n = -1$     | 異常拋出 | 異常拋出 |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

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
