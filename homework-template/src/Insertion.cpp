#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>  // �ɶ����q�\��
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono; // ��K�ϥ� steady_clock

//���J������w�g�ƧǦn����C��
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

//���J�ƧǪk
template <class T>
void InsertionSort(T* a, const int n)
{
    for (int j = 2; j <= n; j++)
    {
        T temp = a[j];
        Insert(temp, a, j - 1);
    }
}

//��ܰO����ϥε{��
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout<<"----------------------------------------------------------"<<endl;
    cout<<"Memory Usage Information:" << endl;
    cout<<"Working Set Size: "<<memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout<<"----------------------------------------------------------"<<endl;
}
//�D�{��
int main(void)
{
    printMemoryUsage();
    int n;
    cout << "�п�J�n�X������:";
    cin >> n;
    int* arr = new int[n + 1]; // arr[0] �O��L��A1~n �����
    int temp = n;
    // �����H����n~1
    for (int i = 1; i <= n; i++)
    {
        arr[i] = temp;
        temp -= 1;
    }

    printMemoryUsage();
    // �}�l�p��
    auto start = steady_clock::now();

    InsertionSort(arr, n);

    // �����p��
    auto end = steady_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << "�ӮɡG" << duration.count() << " �@��" << endl;

    delete[] arr;
    printMemoryUsage();
    return 0;
}