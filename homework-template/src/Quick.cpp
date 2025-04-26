#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <Windows.h>
#include <Psapi.h>
using namespace std;
using namespace chrono;

// ��ܰO����ϥε{��
void printMemoryUsage() {
    PROCESS_MEMORY_COUNTERS memInfo;
    GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo));
    cout << "----------------------------------------------------------" << endl;
    cout << "Memory Usage Information:" << endl;
    cout << "Working Set Size: " << memInfo.WorkingSetSize / 1024 << " KB" << endl;
    cout << "----------------------------------------------------------" << endl;
}

// �D���j QuickSort�]pivot �T�w���̥���^
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
            swap(a[L], a[j]);  // pivot �񤤶�

            // �������j�϶��A�������p�϶�
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

// ���ͩT�w�ɧǸ�ơ]1 �� n�^�Aarr[1] ~ arr[n] �ϥ�
int* generateFixedData(int n) {
    int* arr = new int[n + 2];
    for (int i = 1; i <= n; ++i) {
        arr[i] = i;
    }
    return arr;
}

// �N arr[1..n] �H������
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
    cout << "�п�J�n�X������: ";
    cin >> n;

    int mode;
    cout << "��ܴ���Ҧ� (1 = Average Case, 2 = Worst Case): ";
    cin >> mode;

    // �إ߸��
    int* arr = generateFixedData(n);
    if (mode == 1) {
        permute(arr, n);
        cout << "�ϥ� Average Case ���� (�H������)" << endl;
    }
    else if (mode == 2) {
        cout << "�ϥ� Worst Case ���� (�ɧǱƦC)" << endl;
    }
    else {
        cout << "�ﶵ���~�A�{�������C" << endl;
        delete[] arr;
        return 0;
    }

    printMemoryUsage();

    // �p�ɨñƧ�
    auto start = steady_clock::now();
    QuickSortIterative(arr, 1, n);
    auto end = steady_clock::now();

    auto duration = duration_cast<microseconds>(end - start);
    cout << "�ӮɡG" << duration.count() << " �L��" << endl;

    delete[] arr;
    printMemoryUsage();
    return 0;
}
