#include <iostream>
#include <conio.h>
#include <iomanip>
#include "im_lib.h"

using namespace std;

void task1() {
    int arr_size;

    do {
        cout << "Enter array size (positive multiple of 4): ";
        cin >> arr_size;
        if (arr_size <= 0 || arr_size % 4 != 0) {
            cout << "Error: size must be a positive multiple of 4. Please try again.\n";
        }
    } while (arr_size <= 0 || arr_size % 4 != 0);


    float* array = new float[arr_size];
    float* result_cpp = new float[arr_size];
    float* result_sse = new float[arr_size];


    cout << "Enter " << arr_size << " float elements:\n";
    for (int i = 0; i < arr_size; i++) {
        cout << "arr[" << i << "] = ";
        cin >> array[i];
    }


    for (int i = 0; i < arr_size; i++) {
        result_cpp[i] = array[i] * array[i] * array[i];
    }


    __asm {
        mov esi, array
        mov edi, result_sse
        mov ecx, arr_size
        shr ecx, 2

        p0:
        movdqu xmm0, [esi]
            movdqu xmm1, xmm0
                mulps  xmm0, xmm1
                mulps  xmm0, xmm1
                movdqu[edi], xmm0

                add esi, 16
                add edi, 16
                loop p0
    }

    cout << fixed << setprecision(2);
    cout << "\n" << setw(6) << "Index" << setw(10) << "Value" << setw(12) << "C++ Cube" << setw(12) << "SSE Cube" << "\n";
    for (int i = 0; i < arr_size; i++) {
        cout << setw(6) << i
            << setw(10) << array[i]
            << setw(12) << result_cpp[i]
                << setw(12) << result_sse[i] << '\n';
    }
    delete[] array, result_cpp, result_sse;
    _getch();
}

void task2() {
    int arr_size;
    do {
        cout << "Enter array size (positive multiple of 2): ";
        cin >> arr_size;
        if (arr_size <= 0 || arr_size % 2 != 0) {
            cout << "Error: size must be a positive multiple of 2. Please try again.\n";
        }
    } while (arr_size <= 0 || arr_size % 2 != 0);

    double* array1 = new double[arr_size];
    double* array2 = new double[arr_size];
    double* result_cpp = new double[arr_size];
    double* result_sse = new double[arr_size];
    double min_cpp;
    double min_sse;

    cout << "\nEnter elements of first array (array1):\n";
    for (int i = 0; i < arr_size; i++) {
        cout << "array1[" << i << "] = ";
        cin >> array1[i];
    }

    cout << "\nEnter elements of second array (array2):\n";
    for (int i = 0; i < arr_size; i++) {
        cout << "array2[" << i << "] = ";
        cin >> array2[i];
    }

    min_cpp = array1[0];
    for (int i = 1; i < arr_size; i++) {
        if (array1[i] < min_cpp) min_cpp = array1[i];
    }

    for (int i = 0; i < arr_size; i++) {
        result_cpp[i] = array2[i] * min_cpp;
    }

    __asm {
        mov esi, array1
        mov ecx, arr_size
        shr ecx, 1              //два числа double             
        movupd xmm0, [esi]
            add esi, 16
                dec ecx
                sse_min_loop :
            movapd xmm1, [esi]
                minpd xmm0, xmm1    //XMM0 = min[XMM0, XMM1]
                    add esi, 16
                    dec ecx
                    jnz sse_min_loop
                    shufpd xmm1, xmm0, 1
                    minsd xmm0, xmm1
                    movsd min_sse, xmm0
    }

    __asm {
        mov esi, array2
        mov edi, result_sse
        mov ecx, arr_size
        shr ecx, 1
        movsd xmm2, min_sse
        shufpd xmm2, xmm2, 0    // XMM2 = [XMM2, XMM2] 
        sse_mul_loop:
        movupd xmm0, [esi]
            mulpd xmm0, xmm2
                movupd[edi], xmm0
                add esi, 16
                add edi, 16
                dec ecx
                jnz sse_mul_loop
    }


    cout << fixed << setprecision(4);
    cout << "\nMin (C++) = " << min_cpp << "    Min (SSE) = " << min_sse << "\n";
    cout << setw(8) << "Index" << setw(16) << "C++ result" << setw(16) << "SSE result" << "\n";
    for (int i = 0; i < arr_size; i++) {
        cout << setw(8) << i
            << setw(16) << result_cpp[i]
            << setw(16) << result_sse[i] << "\n";
    }


    delete[] array1;
    delete[] array2;
    delete[] result_cpp;
    delete[] result_sse;

    _getch();
}

void task3() {
    int N, cnt_c = 0, cnt_sse = 0;
    float* array, element;

    do {
        cout << "Enter array size (multiple of 4): ";
        cin >> N;
    } while (N <= 0 || N % 4 != 0);

    array = new float[N];

    for (int i = 0; i < N; i++) {
        cout << "array[" << i << "] = ";
        cin >> array[i];
    }

    cout << "Enter threshold value: ";
    cin >> element;

    for (int i = 0; i < N; i++)
        if (array[i] == element)
            cnt_c++;

    __asm {
        mov   ebx, array
        mov   ecx, N
        shr   ecx, 2

        mov   edx, cnt_sse
        movss xmm1, element
        shufps xmm1, xmm1, 0x00  //XMM1[0,0,0,element] => XMM1[element, el, el, el]

        p0:
        movdqu xmm0, [ebx]
            cmpps  xmm0, xmm1, 0 //Сравнение (1 - истина, 0 - ложь)   
                movmskps eax, xmm0   //В XMM1 записывает копию маски знаков (1-равны, 0-не равны)
                test eax, eax        //Если eax == 0, то нет одинаковых
                jz no_inc

                mov edi, 4
                cnt_bits:
            test eax, 1     //Смотрим младший бит
                jz skip         //Если 0 - skip
                inc edx         //Если 1
                skip :
            shr eax, 1
                dec edi
                jnz cnt_bits

                no_inc :
            add ebx, 16
                loop p0
                mov cnt_sse, edx
    }

    cout << "\nC++ count = " << cnt_c
        << "    SSE count = " << cnt_sse << "\n";

    delete[] array;
    _getch();
}

void task4() {
    int arr_size;
    do {
        cout << "Enter array size (positive integer): ";
        cin >> arr_size;
        if (arr_size <= 0)
            cout << "Error: size must be positive. Please try again.\n";
    } while (arr_size <= 0);

    double* array = new double[arr_size];
    float* result_cpp = new float[arr_size];
    float* result_sse = new float[arr_size];

    cout << "\nEnter " << arr_size << " elements (double):\n";
    for (int i = 0; i < arr_size; i++) {
        cout << "array[" << i << "] = ";
        cin >> array[i];
    }

    for (int i = 0; i < arr_size; i++) {
        result_cpp[i] = static_cast<float>(array[i]);
    }

    __asm {
        mov   esi, array
        mov   edi, result_sse
        mov   ecx, arr_size

        convert_loop :
        movsd xmm0, [esi]          //Один double
            cvtsd2ss xmm1, xmm0    //XMM0 => float
                movss[edi], xmm1

                add   esi, 8
                add   edi, 4
                loop  convert_loop
    }


    cout << fixed << setprecision(7);
    cout << "\nIndex   Double Value        C++ float         SSE float\n";
    for (int i = 0; i < arr_size; i++) {
        cout << setw(5) << i
            << setw(18) << array[i]
            << setw(16) << result_cpp[i]
                << setw(16) << result_sse[i] << "\n";
    }

    delete[] array;
    delete[] result_cpp;
    delete[] result_sse;
    _getch();
}

void dop1() {
    int arr_size = 1;

    cout << "Input array size (multiple of 4):" << endl;
    while (arr_size % 4 != 0)
        cin >> arr_size;

    float* array = new float[arr_size];
    float* array_reverse = new float[arr_size];
    int shift = (4 * arr_size) - (4 * 4);

    for (short i = 0; i < arr_size; i++){
        cout << "array[" << i << "] = ";
        cin >> array[i];
    }

    _asm {
        mov eax, array
        add eax, shift
        mov ebx, array_reverse
        mov ecx, arr_size
        shr ecx, 2

        reverse_loop:
        movdqu xmm1, [eax]
            pshufd xmm1, xmm1, 0x1B
                movdqu[ebx], xmm1
                sub eax, 16
                add ebx, 16
                loop reverse_loop
    }

    cout << "\nInput data:\t" << endl;
    for (short i = 0; i < arr_size; i++)
        cout << " " << array[i];

    cout << "\nResult:\t" << endl;
    for (short i = 0; i < arr_size; i++)
        cout << " " << array_reverse[i];

    delete[] array;
    delete[] array_reverse;

    _getch();
}

void dop2() {
    char src_file[] = "peppers.bmp";
    char proc_file_C[] = "peppers_proc_C.bmp";
    char proc_file_ASM[] = "peppers_proc_ASM.bmp";

    unsigned __int8* src_img;
    unsigned __int8 color_map[1024];
    BmpFileHeader im_header;
    BmpInfoHeader im_info;

    src_img = imread(src_file, im_info, im_header, color_map);
    unsigned long int pixel_number = im_info.height * im_info.width;

    unsigned __int8* proc_img_C = new unsigned __int8[pixel_number];
    unsigned __int8* proc_img_ASM = new unsigned __int8[pixel_number];


    for (int i = 0; i < pixel_number; i++)
        proc_img_C[i] = ~src_img[i];

    _asm {
        mov esi, src_img
        mov edi, proc_img_ASM
        mov ecx, pixel_number
        shr ecx, 4

        xorpd xmm1, xmm1
        cmpps xmm1, xmm1, 0

        image_proc:
        movdqu xmm0, [esi]
            xorpd xmm0, xmm1
                movdqu[edi], xmm0
                add esi, 16
                add edi, 16
                loop image_proc
    }

    imwrite(proc_file_C, proc_img_C, im_info, im_header, color_map);
    imwrite(proc_file_ASM, proc_img_ASM, im_info, im_header, color_map);

    delete[] src_img;
    delete[] proc_img_C;
    delete[] proc_img_ASM;

    _getch();
}

int main() {
    int task = 0;
    while (task != 7) {
        system("cls");
        cout << "\n\tChoose task:" << "\n\tTask 1 - 1" << "\n\tTask 2 - 2" << "\n\tTask 3 - 3" << "\n\tTask 4 - 4" << "\n\tDop task 1 - 5" << "\n\tDop task 2 - 6" << "\n\tExit - 7\n";
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
            task4();
            break;
        case 5:
            dop1();
            break;
        case 6:
            dop2();
            break;
        }
    }
    system("pause");
}
