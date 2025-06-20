#include <iostream>
#include <conio.h>

using namespace std;

void task1() {
    int arr_size;
    cout << "Enter the size of the array: ";
    cin >> arr_size;

    int* array = new int[arr_size];
    for (int i = 0; i < arr_size; i++) {
        cout << "Enter the " << i + 1 << " element of the array: ";
        cin >> array[i];
    }

    int zero_count_c = 0, zero_count_mmx = 0;

    for (int i = 0; i < arr_size; i++) {
        if (array[i] == 0) 
            zero_count_c++;
    }

    int one = 1;

    __asm {
        mov esi, array
        mov ecx, arr_size
        pxor mm1, mm1
        check_loop :
        movd mm0, [esi]
            movd eax, mm0
                cmp eax, 0
                jne skip_inc
                paddd mm1, [one]
                skip_inc :
                    add esi, 4
                    loop check_loop
                    movd zero_count_mmx, mm1
                    emms
    }

    cout << "Number of zero elements in C++: " << zero_count_c << endl;
    cout << "Number of zero elements in MMX: " << zero_count_mmx << endl;

    delete[] array;
    _getch();
}

void task2() {
    int row_size = 1;
    const int col_size = 2;

    cout << "Enter the number of rows of the matrix: ";
    cin >> row_size;
    unsigned int* matrix = new unsigned int[row_size * col_size];
    unsigned int* row_sums_ñ = new unsigned int[row_size];
    unsigned int* row_sums_mmx = new unsigned int[row_size];

    cout << "Enter the elements of the matrix:" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
        {   
            cout << "Enter the matrix element ["<< i <<"]["<< j << "]: ";
            cin >> *(matrix + i * col_size + j);
        }


    cout << endl << "The matrix:" << endl;
    for (int i = 0; i < row_size; i++) {
        cout << "\t";
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix + i * col_size + j) << "\t";
        }
        cout << endl;
    }

    for (int i = 0; i < row_size; i++) {
        row_sums_ñ[i] = 0;
        for (int j = 0; j < col_size; j++) {
            row_sums_ñ[i] += *(matrix + i * col_size + j);
        }
    }

    cout << endl << "The resulting vector string in C++:" << endl << endl << "\t";
    for (int i = 0; i < row_size; i++) {
        cout << row_sums_ñ[i] << "\t";
    }
     
    _asm {
        mov esi, matrix
        mov ebx, row_sums_mmx
        mov ecx, row_size

        p0 :
        movq mm0, [esi]
            add esi, 4
                movq mm1, [esi]
                    paddd mm0, mm1
                        movq[ebx], mm0

                        add esi, 4
                        add ebx, 4

                        loop p0
                        emms
    }
    cout << endl<< endl << "The resulting vector string in MMX:" << endl << endl << "\t";
    for (int i = 0; i < row_size; i++) {
        cout << row_sums_mmx[i] << "\t";
    }
    cout << endl;
    delete[] row_sums_ñ, matrix, row_sums_mmx;
    _getch();
}

void dop1() {
    int arr_size = 8;
    bool check = false;
    while (!check) {
        cout << "Enter the size of the array (Multiple of 4): ";
        cin >> arr_size;
        if (arr_size % 4 == 0 || arr_size < 0)
            check = true;
    }
    
    const int MASK = 0x000000FF;
    int arr_sum = 0;
    short* array = new short[arr_size];

    std::cout << " Input array elements :" << std::endl;
    for (short i = 0; i < arr_size; i++){
        std::cout << " array [" << i << "] = ";
        std::cin >> array[i];
    }
   
    std::cout << " Entered array : \n ";
    for (short i = 0; i < arr_size; i++)
        std::cout << " " << array[i];

    _asm {
        xor esi, esi
        mov esi, array // array -> esi
        mov ecx, arr_size // ecx = array size
        // calculate sum of 4 elements
        lp :
        movq mm0, [esi] // arr [ i ] -> mm 0
            movq mm1, mm0 // arr [ i ] -> mm 1
                psrlq mm1, 16 // mm 1 >> 2
                paddd mm0, mm1 // mm 0 = mm 0 + mm 1
                movq mm1, mm0 // mm 1 = mm 0
                psrlq mm1, 32 // mm 1 >> 4
                pand mm0, MASK // use mask on mm 0
                pand mm1, MASK // use mask on mm 1
                paddd mm0, mm1 // find final sum
                xor eax, eax // accumulator = 0
                movd eax, mm0 // accumulator = sum of 4 elements
                add arr_sum, eax
                // move to next 4 numbers
                add esi, 8
                sub ecx, 4
                jnz lp
                emms
    }
    std::cout << std::endl << " Number of even elements is : " << arr_sum << std::endl;
    _getch();
}

void dop2() {
    int row_size = 1;
    int col_size = 2;
    bool check = false;

    cout << "Enter the number of rows of the matrix: ";
    cin >> row_size;
    while(!check){
        cout << "Enter the number of columns of the matrix: ";
        cin >> col_size;
        if (col_size % 2 == 0 && col_size > 0)
            check = true;
    }

    unsigned int* matrix = new unsigned int[row_size * col_size];
    unsigned int* row_sums_ñ = new unsigned int[row_size];
    unsigned int* row_sums_mmx = new unsigned int[row_size];

    cout << "Enter the elements of the matrix:" << endl;

    for (int i = 0; i < row_size; i++)
        for (int j = 0; j < col_size; j++)
        {
            cout << "Enter the matrix element [" << i << "][" << j << "]: ";
            cin >> *(matrix + i * col_size + j);
        }


    cout << endl << "The matrix:" << endl;
    for (int i = 0; i < row_size; i++) {
        cout << "\t";
        for (int j = 0; j < col_size; j++) {
            cout << *(matrix + i * col_size + j) << "\t";
        }
        cout << endl;
    }

    for (int i = 0; i < row_size; i++) {
        row_sums_ñ[i] = 0;
        for (int j = 0; j < col_size; j++) {
            row_sums_ñ[i] += *(matrix + i * col_size + j);
        }
    }

    cout << endl << "The resulting vector string in C++:" << endl << "\t";
    for (int i = 0; i < row_size; i++) {
        cout << row_sums_ñ[i] << "\t";
    }

    _asm {
        mov esi, matrix
        mov ebx, row_sums_mmx
        mov ecx, row_size

        p0 :
        mov eax, col_size
        movq mm0, [esi]
            sum:
                add esi, 4
                movq mm1, [esi]
                paddd mm0, mm1
                dec eax
                cmp eax, 1
                jne sum
                    movq[ebx], mm0

                    add esi, 4
                    add ebx, 4

                    loop p0
                    emms
    }
    cout << endl << endl << "The resulting vector string in MMX:" << endl << endl << "\t";
    for (int i = 0; i < row_size; i++) {
        cout << row_sums_mmx[i] << "\t";
    }
    cout << endl;
    delete[] row_sums_ñ, matrix, row_sums_mmx;
    _getch();
}

int main() {
    int task = 0;
    while (task != 5) {
        system("cls");
        cout << "\n\tChoose task:" << "\n\tTask 1 - 1" << "\n\tTask 2 - 2" << "\n\tAdditional task 1 - 3" << "\n\tAdditional task 2 - 4" << "\n\tExit - 5\n";
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
            dop1();
            break;
        case 4:
            dop2();
            break;
        }
    }
    system("pause");
}
