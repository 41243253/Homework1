#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>
#include <Windows.h>
#include <Psapi.h>

using namespace std;
using namespace chrono;

// �N arr[1..n] �H������
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

// ��ܥثe�{�����O����ϥα��p
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------" << endl;
    cout << "Memory Usage Information:" << endl;
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout << "----------------------------------------------------------" << endl;
}

// �վ�̤j��n
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


// ��n�Ƨ�
template <class T>
void HeapSort(T* a, const int n)
{
    // �إ߰�n
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
    srand((unsigned)time(nullptr)); // �H���ؤl
    printMemoryUsage();

    int n;
    cout << "�п�J�n�X������G";
    cin >> n;
    int choice;
    cout << "��ܪ��� 1.Average case, 2.Worst case: ";
    cin >> choice;
    // �ʺA�t�m�}�C�Aarr[1..n]
    int* arr = new int[n + 1];
    int* tempArr = new int[n + 1];
    int* worstCaseArr = new int[n + 1];

    printMemoryUsage();

    int testCount;
    // ���զ��ơG�pn�h���A�jn�֦�
    if (n <= 1000) testCount = 1000;
    else if (n <= 10000) testCount = 500;
    else testCount = 10;

    long long worst_time = -1; // �̺C�Ӯɡ]���G�L��^
    long long average_time = 0; // �����Ӯ�(���G�L��)
    // �}�l����
    for (int t = 0; t < testCount; ++t)
    {
        // ��l�� arr �� 1..n
        for (int i = 1; i <= n; ++i) {
            arr[i] = i;
        }
        permute(arr, n); // �H������

        // ��ثe�ƦC�ƥ��� tempArr�]�ƥ���l�ƦC�^
        for (int i = 1; i <= n; ++i) {
            tempArr[i] = arr[i];
        }

        // �}�l�p��
        auto start = steady_clock::now();
        HeapSort(arr, n);
        auto end = steady_clock::now();

        // �p��Ӯɡ]�L��^
        auto duration = duration_cast<microseconds>(end - start).count();
        if (choice == 1) {
            average_time += duration;
        }
        // �p�G�o����e���C�A��s�̮t���p
        if (duration > worst_time)
        {
            worst_time = duration;
            // �� tempArr�]��ƦC�^�s�i worstCaseArr
            for (int i = 1; i <= n; ++i) {
                worstCaseArr[i] = tempArr[i];
            }
        }
    }
    if (choice == 1) {
        average_time /= testCount;
        cout << "�����ӮɡG" << average_time << " �L��" << endl;
    }
    else {
    cout << "�̮t���p�ӮɡG" << worst_time << " �L��" << endl;
    }
    // �N�̮t���p���ƦC�s����r��
    ofstream outFile("worst_case_data.txt");
    if (outFile.is_open())
    {
        for (int i = 1; i <= n; ++i)
        {
            outFile << worstCaseArr[i] << " ";
        }
        outFile.close();
        cout << "�̺C���ƦC��Ƥw�g�x�s�� 'worst_case_data.txt'" << endl;
    }
    else
    {
        cerr << "���~�G�L�k�g�J�ɮסI" << endl;
    }

    delete[] arr;
    delete[] tempArr;
    delete[] worstCaseArr;

    printMemoryUsage();
    return 0;
}
