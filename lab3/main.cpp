#include "wav_lib.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <conio.h>

#pragma inline
using namespace std;

float calculate_cpp(float x1, float x2) {
    if (x2 <= 5) {
        cout << "Error" << endl;
        return 1;
    }
    return (sqrt(x1 + 8) / sqrt(x2 - 5)) * pow(cos(x1 + (3.141592 / 4)), 2);

}

void task1() {
    float x1, x2, y_asm = 0, y_c = 0;
    cout << "������� x1: ";
    cin >> x1;
    while (true) {

        cout << "������� x2 (x2 > 5): ";
        cin >> x2;

        if (x2 > 5) {
            break;
        }
        else {
            cout << "������: x2 < 5." << endl;

        }
    }
    y_c = calculate_cpp(x1, x2);

    float eight = 8;
    float five = 5;
    float four = 4;

    __asm {
        finit               // ������������� FPU

        fld x1              //ST(0) = x1
        fld eight           //ST(0) = 8,                ST(1) = x1
        fadd                //ST(0) = x1+8
        //fiadd eight
        fsqrt               //ST(0) = Res1

        fld x2            //ST(0) = 5,                ST(1) = Res1
        fld five              //ST(0) = x2,               ST(1) = 5,     ST(2) = Res1
        fsub                //ST(0) = x2-5,             ST(1) = Res1

        fsqrt               //ST(0) = Res2,             ST(1) = Res1

        fdiv                //ST(0) = Res12

        fld x1              //ST(0) = x1,               ST(1) = Res12
        fldpi               //ST(0) = pi,               ST(1) = x1,     ST(2) = Res12
        fld four            //ST(0) = 4,                ST(1) = pi,     ST(2) = x1,     ST(3) = Res12
        fdiv                //ST(0) = pi/4,             ST(1) = x1,     ST(2) = Res12
        fadd ST(0), ST(1)   //ST(0) = x1+(pi/4),        ST(1) = Res12
        fcos                //ST(0) = cos(x1+pi/4),     ST(1) = Res12
        fmul ST(0), ST(0)
        //fscale            //ST(0) = cos^2(x1+pi/4),   ST(1) = Res12

        fmul ST(0), ST(2)   //ST(0) = Res123

        fstp y_asm
    }

    cout << "\n��������� �� C++:";
    cout << "\ny_C++ = " << y_c << endl;

    cout << "\n��������� �� FPU:";
    cout << "\ny_asm = " << y_asm << endl;
    _getch();
}

void task2() {
    int n;
    float c_diff = 0.0;
    float fpu_diff = 0.0;
    bool firstNegativeFound = false;

    cout << "������� ������ �������: ";
    cin >> n;

    float* c_arr = new float[n];
    float* fpu_arr = new float[n];

    cout << "\n������� �������� �������: \n";
    for (int i = 0; i < n; i++) {
        cout << "arr [" << i << "]: ";
        cin >> c_arr[i];
        fpu_arr[i] = c_arr[i];
    }

    cout << "\n���������� ������:\n";
    for (int i = 0; i < n; i++)
        cout << c_arr[i] << " ";

    for (int i = 0; i < n; i++) {
        if (c_arr[i] < 0) {
            if (!firstNegativeFound) {
                firstNegativeFound = true;
                c_diff += c_arr[i];
            }
            else
                c_diff -= c_arr[i];
        }
    }

    float c_mod = std::abs(c_diff);

    for (int i = 0; i < n; i++) {
        if (i % 2 != 0)
            c_arr[i] = c_mod;
    }

    float fpu_mod = fpu_diff;
    _asm {
        mov ebx, fpu_arr       // ����� ������� � EBX
        mov ecx, n             // ������ ������� � ECX
        finit                  // ������������� FPU
        fldz                   // ST(0) = 0 (���� ���������� �������������)
        xor edx, edx           // ������� ������������� �����

        process_loop :
        fld dword ptr[ebx]     // ST(0) = ������� �������
            ftst                   // ��������� ���� ST(0)
                fstsw ax               // ��������� ������ FPU
                sahf                   // ��������� ����� � EFLAGS
                jnb not_negative       // ���� �� �������������, ����������

                // ������������� �����:
                inc edx                // ����������� �������
                cmp edx, 1             // ������ �� ��� �������������?
                jne not_first          // ���� ���, ��������� � ���������

                // ������ ������������� ����� - ��������� ��� ������
                fstp st(1)             // �������� 0 �� ������� ��������
                jmp next_element

                not_first :
            // ��������� ��������: ST(1) = ST(1) - ST(0)
            fsubp st(1), st(0)
                jmp next_element

                not_negative :
            fstp st(0)             // ������� ��������������� �����

                next_element :
                add ebx, 4             // ��������� �������
                loop process_loop      // ��������� ����

                // ��������� ���������
                fstp fpu_diff
                fwait                  // �������������
    }
    _asm {
        mov ebx, fpu_arr       // ����� ������� � EBX
        mov ecx, n             // ������ ������� � ECX
        xor edx, edx           // ������� ������� (�������� � 0)

        finit                  // ������������� FPU
        fld fpu_diff           // ��������� ����������� ��������
        fabs                   // ����� ������ �������� (ST(0) = |fpu_diff|)

        modify_loop :
        test edx, 1            // ��������� ������� ��� (��������)
            jz even_index          // ���� ������ ������, ����������

            // ��� �������� ��������:
            fst dword ptr[ebx]    // �������� ������� �� |fpu_diff|

            even_index :
                add ebx, 4             // ��������� � ���������� ��������
                inc edx                // ����������� ������� �������
                loop modify_loop       // ��������� ��� ���� ���������

                fwait                  // �������������
    }
    cout << "\n\n�������� ������������� ��������� �� FPU: " << fpu_diff;
    cout << "\n\n���������� ������ �� FPU:\n";
    for (int i = 0; i < n; i++)
        cout << fpu_arr[i] << " ";

    cout << "\n\n�������� ������������� ��������� �� C++: " << c_diff;
    cout << "\n\n���������� ������ �� C++:\n";
    for (int i = 0; i < n; i++)
        cout << c_arr[i] << " ";
    cout << endl << endl;

    delete[] fpu_arr, c_arr;
    _getch();
}

void task3() {
    WAVInfo info;

    float alpha = 0.999;
    int* x, * y_cpp, * y_fpu;
    
    x = wavread("var_DC.wav", info);
    y_cpp = new int[info.NumberOfSamples];
    y_fpu = new int[info.NumberOfSamples];
    memset(y_cpp, 0, sizeof(int) * info.NumberOfSamples);
    memset(y_fpu, 0, sizeof(int) * info.NumberOfSamples);

    int init_samples = std::min(200u, info.NumberOfSamples); // u = unsigned
    float x_ave_pred = 0;
    for (int i = 0; i < init_samples; ++i) {
        x_ave_pred += x[i];
    }
    x_ave_pred /= init_samples;
    float temp = x_ave_pred;

    float diff = 1 - alpha;
    for (unsigned int n = 0; n < info.NumberOfSamples; n++) {
        float x_ave = static_cast<float>(x[n]) * diff + x_ave_pred * alpha;
        x_ave_pred = x_ave;
        y_cpp[n] = static_cast<float>(x[n]) - x_ave;
    }

    _asm {
        mov ecx, 0  // �������� � n = 0
        mov esi, x  // ��������� �� ������ x
        mov edi, y_fpu  // ��������� �� ������ y_fpu

        outer_loop :
            cmp ecx, info.NumberOfSamples
            jge end_loop    // ���� n >= NumberOfSamples, ��������� ����

            fild dword ptr[esi + ecx * 4]    // ��������� x[n] � ���� FPU
            fld diff     // ���������� (1 - alpha) � ���� FPU
            fmul    // �������� x[n] �� (1 - alpha)

            fld temp    // ���������� temp � ���� FPU
            fld alpha   // ���������� alpha � ���� FPU
            fmul    // �������� temp �� alpha
            fadd    // ���������� (x[n] * (1 - alpha)) � (temp * alpha) = x_ave
            
            fst temp    // ��������� x_ave � temp
            
            fild dword ptr[esi + ecx * 4]    // ��������� x[n] � ���� FPU
            fsub        // �������� x_ave �� x[n]

            fistp dword ptr[edi + ecx * 4]  // ��������� ��������� � y_fpu[n]
            inc ecx
            jmp outer_loop

        end_loop:
            fwait
    }

    wavwrite("var_DC_new_cpp.wav", info, y_cpp);
    wavwrite("var_DC_new_fpu.wav", info, y_fpu);

    cout << "\n\t������� ����������\n\n";
    delete[] x, y_cpp, y_fpu;
    _getch();
}

float calculate_cpp_dop(float x, float y) {
    return (cos(x) / (y * log2(x + 1))) * sqrt(sin(y) * pow(x, y)) + exp(y) - tan(x / y) - pow(2, x - 1);
}

void dop() {
    float x, y, R_asm = 0, R_c = 0;
    cout << "������� x: ";
    cin >> x;
    cout << "������� y: ";
    cin >> y;
    R_c = calculate_cpp_dop(x, y);

    float two = 2;

    __asm {
        finit               // ������������� FPU

        fld x               //ST(0) = x
        fcos                //ST(0) = cos(x)

        fld x               //ST(0) = x       ST(1) = cos(x)   
        fld1                //ST(0) = 1       ST(1) = x       ST(2) = cos(x)
        fadd                //ST(0) = x+1     ST(1) = cos(x)
        fld1                //ST(0) = 1       ST(1) = x+1     ST(2) = cos(x)
        fxch                //ST(0) = x+1     ST(1) = 1       ST(2) = cos(x)
        fyl2x               //ST(0) = log     ST(1) = cos(x)
        fld y               //ST(0) = y       ST(1) = log     ST(2) = cos(x)
        fmul                //ST(0) = y*log   ST(2) = cos(x)

        fdiv                //ST(0) = div

        fld y               //ST(0) = y                       ST(1) = div
        fsin                //ST(0) = sinY                    ST(1) = div

        //x^y
        fld y               //ST(0) = y       ST(1) = sinY    ST(2) = div
        fld x               //ST(0) = x       ST(1) = y       ST(2) = sinY      ST(3) = div
        fyl2x               //ST(0) = y * log2(x) = a         ST(1) = sinY      ST(2) = div   
        fld st(0)           //ST(0) = a       ST(1) = a       ST(2) = sinY      ST(3) = div
        frndint             //ST(0) = Aint    ST(1) = a       ST(2) = sinY      ST(3) = div
        fsub ST(1), ST(0)   //ST(0) = Aint    ST(1) = Afract  ST(2) = sinY      ST(3) = div
        fxch                //ST(0) = Afract  ST(1) = Aint    ST(2) = sinY      ST(3) = div
        f2xm1               //ST(0) = 2^a-1   ST(1) = Aint    ST(2) = sinY      ST(3) = div
        fld1                //ST(0) = 1       ST(1) = 2^a-1   ST(2) = Aint      ST(3) = sinY      ST(4) = div
        fadd                //ST(0) = 2^a     ST(1) = Aint    ST(2) = sinY      ST(3) = div
        fscale              //ST(0) = 2^a*2^A ST(1) = Aint    ST(2) = sinY      ST(3) = div
        fstp ST(1)          //ST(0) = x^y     ST(1) = sinY    ST(2) = div         

        fmul                //ST(0) = *       ST(1) = div
        fsqrt               //ST(0) = sqrt    ST(1) = div        

        fmul                //ST(0) = div * sqrt (=res1)

        // 2^(x-1)
        fld x               //ST(0) = x       ST(1) = res1
        fld1                //ST(0) = 1       ST(1) = x       ST(2) = res1
        fsub                //ST(0) = x-1     ST(1) = res1              
        fld1                //ST(0) = 1       ST(1) = x-1     ST(2) = res1
        fscale              //ST(0) = 1*2^(x-1) 

        // tan
        fld x               //ST(0) = x       ST(1) = 2^(x-1) ST(2) = res1
        fld y               //ST(0) = y       ST(1) = x       ST(2) = 2^(x-1) ST(3) = res1
        fdiv                //ST(0) = x/y     ST(1) = 2^(x-1) ST(2) = res1
        fptan               //ST(0) = 1       ST(1) = tan     ST(2) = 2^(x-1) ST(3) = res1
        ffree st(0)         //ST(0) = tan     ST(1) = 2^(x-1) ST(2) = res1       
        fincstp

        // exp
        fld y               //ST(0) = y       ST(1) = tan     ST(2) = 2^(x-1) ST(3) = res1
        fldl2e              //ST(0) = log2(e) ST(1) = y       ST(2) = tan     ST(3) = 2^(x-1)   ST(4) = res1
        fmul                //ST(0) = y * log2(e) = a         ST(1) = tan     ST(2) = 2^(x-1)   ST(3) = res1
        fld st(0)           //ST(0) = a       ST(1) = a       ST(2) = tan     ST(3) = 2^(x-1)   ST(4) = res1
        frndint             //ST(0) = int(a)  ST(1) = a       ST(2) = tan     ST(3) = 2^(x-1)   ST(4) = res1
        fsub st(1), st(0)   //ST(1) = frac(a) = a - int(a)    ST(0) = int(a)  ST(2) = tan       ST(3) = 2^(x-1)   ST(4) = res1
        fxch                //ST(0) = frac(a) ST(1) = int(a)  ST(2) = tan     ST(3) = 2^(x-1)   ST(4) = res1
        f2xm1               //ST(0) = 2 ^ frac(a) - 1         ST(1) = int(a)  ST(2) = tan       ST(3) = 2^(x-1)   ST(4) = res1
        fld1                //ST(0) = 1       ST(1) = 2 ^ frac(a) - 1         ST(2) = int(a)    ST(3) = tan       ST(4) = 2^(x-1)   ST(5) = res1
        fadd                //ST(0) = 2 ^ frac(a)             ST(1) = int(a)  ST(2) = tan       ST(3) = 2^(x-1)   ST(4) = res1
        fscale              //ST(0) = 2 ^ frac(a) * 2 ^ int(a) = 2 ^ a = e^y  ST(1) = tan       ST(2) = 2^(x-1)   ST(3) = res1 

        // ST(0) = e^y  ST(1) = 4(?)  ST(2) = tan  ST(3) = 2^(x-1)  ST(4) = 1(?)  ST(5) = res1
        fsub st(0), st(2)   //ST(0) = e^y-tan                 ST(1) = 2^(x-1) ST(2) = res1
        fsub st(0), st(3)   //ST(0) = e^y-tan-2^(x-1)         ST(1) = res1

        fadd st(0), st(5)   //ST(0) = Res

        fstp R_asm
    }

    cout << "\n��������� �� C++:" << endl;
    cout << "R_C++ = " << R_c << endl;

    cout << "\n��������� �� FPU:" << endl;
    cout << "R_asm = " << R_asm << endl;
    _getch();
}

int main() {
    setlocale(LC_ALL, "Russian");

    int task = 0;
    while (task != 5) {
        system("cls");
        cout << "\n\t��������� �������:" << "\n\t������� 1 - 1" << "\n\t������� 2 - 2" << "\n\t������� 3 - 3" << "\n\t�������������� ������� - 4" << "\n\t����� - 5\n";
        cin >> task;
        system("cls");
            switch (task) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
           case 3:
               task3();
               break;
           case 4:
               dop();
               break;
           }
    }
    system("pause");
}