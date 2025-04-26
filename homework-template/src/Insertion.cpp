#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>  // 時間測量功能
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono; // 方便使用 steady_clock

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
//主程式
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

    cout << "耗時：" << duration.count() << " 毫秒" << endl;

    delete[] arr;
    printMemoryUsage();
    return 0;
}