#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// ��ܰO����ϥε{��
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS m;
    GetProcessMemoryInfo(GetCurrentProcess(), &m, sizeof(m));
    cout << "----------------------------------------------------------\n"
        << "Memory Usage Information:\n"
        << "Working Set Size: " << m.WorkingSetSize / 1024 << " KB\n"
        << "----------------------------------------------------------\n";
}

// �H������ arr[1..n]
void permute(int* arr, int n) {
    for (int i = n; i >= 2; --i) {
        int j = rand() % i + 1;
        swap(arr[i], arr[j]);
    }
}

// �X�� [l..m] �M [m+1..r]
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

// ���j Merge Sort
void mergeSort(int* a, int l, int r, int* tmp) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(a, l, m, tmp);
    mergeSort(a, m + 1, r, tmp);
    merge(a, l, m, r, tmp);
}

int main() {
    srand((unsigned)time(nullptr));
    int loopcount;
    cout << "�`���X��: ";
    cin >> loopcount;
    int n;
    cout << "�п�J�n�X������: ";
    cin >> n;
    printMemoryUsage();
    while (loopcount > 0) {
        loopcount -= 1;

        // 1) �إ� 1..n �å���
        int* arr = new int[n + 1];
        for (int i = 1; i <= n; ++i) arr[i] = i;
        permute(arr, n);
        cout << "�w�ͦ����H������ " << n << " ������C\n";

       // printMemoryUsage();

        // 2) �O�d�u��l���Ƨǡv�ƥ�
        int* orig = new int[n + 1];
        memcpy(orig + 1, arr + 1, n * sizeof(int));

        // 3) ���U�}�C
        int* tmp = new int[n + 1];

        // 4) Merge Sort �p��
        auto start = steady_clock::now();
        mergeSort(arr, 1, n, tmp);
        auto end = steady_clock::now();
        long long dur = duration_cast<microseconds>(end - start).count();
        cout << "Merge Sort �Ӯ�: " << dur << " �L��\n";

       // printMemoryUsage();

        // 5) ��s max_data.txt�]�u�g��l���ƧǸ�ơ^
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
            cout << "�w��s " << F << "�]�t��l���ƧǴ���^�C\n";
        }
        else {
            cout << "�ثe�̤j�Ӯ� " << prev << " �L��A����s�ɮסC\n";
        }

        delete[] arr;
        delete[] orig;
        delete[] tmp;
    }
    printMemoryUsage();
    return 0;
}
