//. Порядок: по неубыванию элементов. Методы: выбора, пузырька с фиксацией места
//обмена, быстрая сортировка, естественное слияние. N1=5000, N2=30000, N3=100000, N4=120000.
//Критерий – количество присваиваний.

#include <iostream>
#include <fstream>
#include <cmath>

#define N1 5000
#define N2 30000
#define N3 100000
#define N4 120000

using namespace std;

struct Information {
    unsigned long int addOnMemory = 0;
    unsigned long int arrayAssignment = 0;
    unsigned long int otherAssignment = 0;
};
typedef struct Information info;

void reverseArray (int *a, int n) { //переворачивает массив
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        a[i] = a[i] + a[j];
        a[j] = a[i] - a[j];
        a[i] = a[i] - a[j];
    }
}

info StraightSelection (int *a, int n) {
    info reg;
    int i, j, tmp, *min;
    reg.addOnMemory += sizeof(int) * 3 + sizeof(int*);
    reg.otherAssignment++; // i = 0
    for (i = 0; i < n-1; i++) {
        min = a + i;
        reg.otherAssignment += 3; // i++, min =, j =
        for (j = i + 1; j < n; j++) {
            reg.otherAssignment++; // j++
            if (a[j] < *min) {
                min = a + j;
                reg.otherAssignment++; // min =
            }
        }
        tmp = *min;
        *min = a[i];
        reg.otherAssignment += 2; // tmp = , *min =
        a[i] = tmp;
        reg.arrayAssignment++; //a[i]
    }
    return reg;
}

info BubbleSort (int *a, int n) {
    info reg;
    int i = 1, j, x, k;
    reg.addOnMemory = sizeof(int) * 4;
    reg.otherAssignment++; // i =
    do {
        k = 0;
        reg.otherAssignment += 2; // k =, j =
        for (j = n-1; j >= i; j--) {
            reg.otherAssignment++; // j--
            if (a[j-1] > a[j]) {
                x = a[j-1];
                reg.otherAssignment++; // x =
                a[j - 1] = a[j];
                a[j] = x;
                reg.arrayAssignment += 2; // a[j - 1] =, a[j]
                k = j;
                reg.otherAssignment++; // k =
            }
        }
        reg.otherAssignment++; // i =
        i = k;
    }
    while (k);
    return reg;
}

info QuickSort (int *a, int n) {
    info reg;
    const int M = log(n) / log(2) + 1;
    reg.otherAssignment++; //M =
    reg.addOnMemory += sizeof(const int);

    int i, j, left, right, s, x, w;
    reg.addOnMemory += sizeof(int) * 7;

    struct Stack {
        int left, right;
    } *stack;
    reg.addOnMemory += sizeof(struct Stack) + sizeof(struct Stack*);

    stack = new struct Stack[M];
    s = 0;
    stack[0].left = 0;
    stack[0].right = n - 1;
    reg.otherAssignment += 4; //stack =, s = , stack.left = , stack.right =
    do {
        left = stack[s].left;
        right = stack[s].right;
        s--;
        reg.otherAssignment += 3; //left = , right = , s--
        do {
            i = left;
            j = right;
            x = a[(left + right) / 2];
            reg.otherAssignment += 3; // i = , j = , x = ;
            do {
                while (a[i] < x) {
                    i++;
                    reg.otherAssignment++; // i++
                }
                while (x < a[j]) {
                    j--;
                    reg.otherAssignment++; // j--
                }
                if (i <= j) {
                    w = a[i];
                    reg.otherAssignment++; // w =
                    a[i] = a[j];
                    a[j] = w;
                    reg.arrayAssignment += 2; // a[i] = , a[j] =
                    i++;
                    j--;
                    reg.otherAssignment += 2; // i++, j--
                }
            }
            while (i < j);

            if (i < right && right - i >= j - left) { 
                s++;
                stack[s].left = i;
                stack[s].right = right;
                right = j; 
                reg.otherAssignment += 4; // s++, stack.left =, stack.right = , right =
            } else if (j > left && j - left > right - i) { 
                s++;
                stack[s].left = left;
                stack[s].right = j;
                left = i; 
                reg.otherAssignment += 4; // s++, stack.left =, stack.right = , left =
            } else {
                reg.otherAssignment++; // left =
                left = right; 
            }
        }
        while (left < right);
    }
    while (s > -1);
    free (stack);
    return reg;
}

info NaturalMerge(int *a, int n) {
    info reg;
    int arrayRear = n;
    int arrayHead = 0;
    int inHead[2], inRear[2];
    int outHead[2], outRear[2];
    int *in[2], *out[2];
    int i, k, ks, n1, n2, flag1, flag2, x;

    reg.addOnMemory += sizeof(int) * 10 + sizeof(int[2]) * 4 + sizeof(int*) * 4;
    reg.otherAssignment += 3; // arrayRear =, arrayHead =, i =
    for (i = 0; i < 2; i++) {
        in[i] = new int [arrayRear];
        inHead[i] = 0;
        inRear[i] = 0;
        out[i] = new int[arrayRear];
        outHead[i] = 0;
        outRear[i] = 0;
        reg.otherAssignment += 7; // i++, in =, inHead =, inRear =, out = , outHead = , outRear =
    }

    if (!in[0] || !in[1] || !out[0] || !out[1]) {
        return reg;
    }

    ks = 0;
    k = 0;
    n2 = a[arrayHead++];
    reg.otherAssignment += 4; // ks = , k = , n2 = , arrayHead++
    while (arrayHead < arrayRear) {
        out[k][outRear[k]++] = n2;
        reg.otherAssignment += 2; //out = , outRear++
        while (arrayHead < arrayRear) {
            n1 = a[arrayHead++];
            reg.otherAssignment += 2; //arrayHead++, n1 =
            if (n1 >= n2) {
                out[k][outRear[k]++] = n1;
                n2 = n1;
                reg.otherAssignment += 3; //n2 = , out = , outRear++
            } else break;
        }

        ks++;
        k = !k;
        reg.otherAssignment += 2;
        if (arrayHead >= arrayRear && n1 < n2) {
            out[k][outRear[k]++] = n1;
            ks++;
            reg.otherAssignment += 3; // outRear =, ks++, out =
            break;
        }
        reg.otherAssignment++; // n2 =
        n2 = n1;
    }
    while (ks > 1) {
        reg.otherAssignment++; // i =
        for (i = 0; i < 2; i++) {
            reg.otherAssignment++; // i++
            int *pt = in[i];
            reg.addOnMemory += sizeof(int*);
            in[i] = out[i];
            out[i] = pt;
            inHead[i] = 0;
            outHead[i] = 0;
            inRear[i] = outRear[i];
            outRear[i] = 0;
            reg.otherAssignment += 6; // in =, out = , inHead =, outHead =, inRear = , outRear =
        }

        k = 0;
        ks = 0;
        flag1 = inHead[0] < inRear[0];
        reg.otherAssignment += 3; // k = , ks = , flag1 =
        if (flag1)
            n1 = in[0][inHead[0]++];
        flag2 = inHead[1] < inRear[1];
        if (flag2)
            n2 = in[1][inHead[1]++];
        if (n1 < n2)
            x = n1;
        else x = n2;

        while (flag1 && flag2) {
            if (n1 < x&& n2 < x) {
                ks++;
                k = !k;
                reg.otherAssignment += 2; //ks++, k =
            }

            if (n1 <= n2) {
                out[k][outRear[k]++] = n1;
                x = n1;
                flag1 = inHead[0] < inRear[0];
                reg.otherAssignment += 4; // out = , x = , flag1 = , outRear++
                if (flag1) {
                    n1 = in[0][inHead[0]++];
                    reg.otherAssignment += 2; // n1 = , inHead++
                }
            } else {
                out[k][outRear[k]++] = n2;
                x = n2;
                flag2 = inHead[1] < inRear[1];
                reg.otherAssignment += 4; // out = , x = , flag2 = , outRear++
                if (flag2) {
                    n2 = in[1][inHead[1]++];
                    reg.otherAssignment += 2; // n2 =, inHead++
                }
            }

            if ( ( n1 < x || !flag1 ) && n2 >= x && flag2)
                do {
                    out[k][outRear[k]++] = n2;
                    x = n2;
                    flag2 = inHead[1] < inRear[1];
                    reg.otherAssignment += 4; // out = , x = , flag2 = , outRear++
                    if (flag2) {
                        n2 = in[1][inHead[1]++];
                        reg.otherAssignment += 2; // n2 =, inHead++
                    }
                }
                while (n2 >= x&& flag2);
            if ( ( n2 < x || !flag2 ) && n1 >= x && flag1)
                do {
                    out[k][outRear[k]++] = n1;
                    x = n1;
                    flag1 = inHead[0] < inRear[0];
                    reg.otherAssignment += 4; //out = , x = , flag1 = , outRear++
                    if (flag1) {
                        n1 = in[0][inHead[0]++];
                        reg.otherAssignment += 2; // n1 =, inHead++
                    }
                }
                while (n1 >= x && flag1);
        }

        ks++;
        k = !k;
        reg.otherAssignment += 2; // ks++, k =
        if (flag1) {
            out[k][outRear[k]++] = n1;
            reg.otherAssignment += 2; // out =, outRear++
            while (inHead[0] < inRear[0]) {
                n1 = in[0][inHead[0]++];
                out[k][outRear[k]++] = n1;
                reg.otherAssignment += 4; // n1 =, inHead++, outRear++, out =
            }
            ks++;
            reg.otherAssignment++; // ks++
        }

        if (flag2) {
            out[k][outRear[k]++] = n2;
            reg.otherAssignment += 2; // out = , outRear++
            while (inHead[1]  < inRear[1]) {
                n1 = in[1][inHead[1]++];
                out[k][outRear[k]++] = n1;
                reg.otherAssignment += 4; // n1 =, inHead++, outRear++, out =
            }
            ks++;
            reg.otherAssignment++; //ks++
        }
    }

    reg.otherAssignment++; // i =
    for (i = 0; i < n; i++) {
        a[i] = out[0][i];
        reg.otherAssignment++; // i++
        reg.arrayAssignment++; // a =
    }
    return reg;
}

void print(info (*f)(int*, int), int *a, int n) {
    info reg;
    reg = f(a, n);
    printf("Random  |%19d |%16lu |%17lu |%8lu|\n", n, reg.arrayAssignment + reg.otherAssignment, reg.arrayAssignment,
           reg.addOnMemory);
    reg = f(a, n);
    printf("Direct  |%19d |%16lu |%17lu |%8lu|\n", n, reg.arrayAssignment + reg.otherAssignment, reg.arrayAssignment,
           reg.addOnMemory);
    reverseArray(a, n);
    reg = f(a, n);
    printf("Reverse |%19d |%16lu |%17lu |%8lu|\n", n, reg.arrayAssignment + reg.otherAssignment, reg.arrayAssignment,
           reg.addOnMemory);
}

void sorting(info (*f)(int*, int)) {
    fstream File;
    File.open("numbers.txt");
    int *arrN1 = new int[N1];
    int *arrN2 = new int[N2];
    int *arrN3 = new int[N3];
    int *arrN4 = new int[N4];

    int i = 0;
    int number;
    while (File >> number){
        arrN4[i] = number;
        if ( i < 100000) {
            arrN3[i] = number;
            if ( i < 30000) {
                arrN2[i] = number;
                if (i < 5000) {
                    arrN1[i] = number;
                }
            }
        }
        i++;
    }

    cout << "Array   | Amount of elements | All assignments | Array assignment | Memory |" << endl;
    print(f, arrN1, 50);
    delete[] arrN1;
    print(f, arrN2, 300);
    delete[] arrN2;
    print(f, arrN3, 1000);
    delete[] arrN3;
    print(f, arrN4, 1200);
    delete[] arrN4;
}


int main() {
    cout << "Straight Selection:" << endl;
    sorting(StraightSelection);
    cout << endl;

    cout << "BubbleSort (fixation of the place of exchange):" << endl;
    sorting(BubbleSort);
    cout << endl;

    cout << "Quick Sort:" << endl;
    sorting(QuickSort);
    cout << endl;

    cout << "Natural Merge:" << endl;
    sorting(NaturalMerge);
    cout << endl;
    return 0;
}