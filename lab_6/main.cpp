#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <chrono>
#include <ctime>
#include <conio.h>
#include <algorithm>
#include "im_lib.h"
#include <iomanip>
using namespace std;

double calculate_open(double X, int N, int num_threads) {
    double denominator = 0.0;

#pragma omp parallel for num_threads(num_threads) reduction(+:denominator) schedule(dynamic)
    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= 6 * N + i; ++j) {
            denominator += (j + pow(5 * X + 6 * j, 4.0 / 11.0)) / (2.0 * i * j - 1.0);
        }
    }

    return static_cast<double>(N) / denominator;
}
double calculate_seq(double X, int N) {
    double denominator = 0.0;

    for (int i = 1; i <= N; ++i) {
        for (int j = i; j <= 6 * N + i; ++j) {
            denominator += (j + pow(5 * X + 6 * j, 4.0 / 11.0)) / (2.0 * i * j - 1.0);
        }
    }

    return static_cast<double>(N) / denominator;
}
void task1() {
    int N;
    double X;
    int num_threads;

    cout << "Enter value of X: ";
    cin >> X;

    cout << "Enter N (number of iterations): ";
    cin >> N;

    cout << "Enter number of threads: ";
    cin >> num_threads;

    double start_time = omp_get_wtime();
    double result_open = calculate_open(X, N, num_threads);
    double end_time = omp_get_wtime();
    double time_open = end_time - start_time;

    start_time = omp_get_wtime();
    double result_seq = calculate_seq(X, N);
    end_time = omp_get_wtime();
    double time_seq = end_time - start_time;

    cout << "\nComputation Results:\n";
    cout << "Parallel   : F(X, N) = " << result_open << ", Time = " << time_open << " seconds\n";
    cout << "Sequential : F(X, N) = " << result_seq << ", Time = " << time_seq << " seconds\n";
    _getch();
}

void initialize_array(vector<double>& array) {
    for (auto& elem : array) {
        elem = static_cast<double>(rand()) / RAND_MAX * 100.0;
    }
}
double sequential_cube_sum(const vector<double>& A) {
    double sum = 0.0;
    for (size_t i = 0; i < A.size(); ++i) {
        sum += pow(A[i], 3);
    }
    return sum;
}
double parallel_cube_sum(const vector<double>& A, int num_threads) {
    vector<double> local_sums(num_threads, 0.0);

#pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        int chunk_size = A.size() / num_threads;
        int start = tid * chunk_size;
        int end = (tid == num_threads - 1) ? A.size() : start + chunk_size;

        for (int i = start; i < end; ++i) {
            local_sums[tid] += pow(A[i], 3);
        }
    }


    double total = 0.0;
    for (int i = 0; i < num_threads; ++i) {
        total += local_sums[i];
    }
    return total;
}
void task2() {
    srand(static_cast<unsigned>(time(nullptr)));

    int array_size;
    int num_threads;

    cout << "Enter array size: ";
    cin >> array_size;
    cout << "Enter number of threads: ";
    cin >> num_threads;

    vector<double> A(array_size);
    initialize_array(A);

    double start, end;

    start = omp_get_wtime();
    double seq_sum = sequential_cube_sum(A);
    end = omp_get_wtime();
    double time_seq = end - start;

    start = omp_get_wtime();
    double par_sum = parallel_cube_sum(A, num_threads);
    end = omp_get_wtime();
    double time_par = end - start;


    cout << "\nSequential sum of cubes: " << seq_sum << endl;
    cout << "Parallel sum of cubes:   " << par_sum << endl;

    cout << "\nSequential time: " << time_seq << " s" << endl;
    cout << "Parallel time:   " << time_par << " s" << endl;
    _getch();
}

double round1(double value) {
    return std::round(value * 10.0) / 10.0;
}
int sequential_threshold_count(const vector<double>& A, double threshold) {
    int count = 0;
    double rounded_threshold = round1(threshold);
    for (size_t i = 0; i < A.size(); ++i) {
        double rounded_A = round1(A[i]);
        if (rounded_A == rounded_threshold) {
            ++count;
        }
    }
    return count;
}
int parallel_threshold_count(const vector<double>& A, double threshold, int num_threads) {
    int count = 0;
    double rounded_threshold = round1(threshold);

#pragma omp parallel for reduction(+:count) num_threads(num_threads)
    for (int i = 0; i < A.size(); ++i) {
        double rounded_A = round1(A[i]);
        if (rounded_A == rounded_threshold) {
            ++count;
        }
    }
    return count;
}
void task3() {
    srand(static_cast<unsigned>(time(nullptr)));

    int array_size;
    int num_threads;
    double threshold;

    cout << "Enter array size: ";
    cin >> array_size;
    cout << "Enter number of threads: ";
    cin >> num_threads;
    cout << "Enter threshold value: ";
    cin >> threshold;

    vector<double> A(array_size);
    initialize_array(A);

    double start, end;

    start = omp_get_wtime();
    int seq_count = sequential_threshold_count(A, threshold);
    end = omp_get_wtime();
    double time_seq = end - start;

    start = omp_get_wtime();
    int par_count = parallel_threshold_count(A, threshold, num_threads);
    end = omp_get_wtime();
    double time_par = end - start;

    cout << "\nSequential count of elements equal to threshold: " << seq_count << endl;
    cout << "Parallel count of elements equal to threshold:   " << par_count << endl;
    cout << "\nSequential time: " << time_seq << " s" << endl;
    cout << "Parallel time:   " << time_par << " s" << endl;
    _getch();
}

double factorial(int n) {
    if (n <= 1) return 1.0;
    double result = 1.0;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}
void sequential_processing(const vector<double>& arr) {
    vector<double> rounded(arr.size());
    vector<double> factorial_arr(arr.size());
    vector<double> tan_arr(arr.size());

    double start_time = omp_get_wtime();

    for (size_t i = 0; i < arr.size(); ++i)
        rounded[i] = round(arr[i]);

    for (size_t i = 0; i < arr.size(); ++i) {
        int val = static_cast<int>(round(arr[i]));
        factorial_arr[i] = (val >= 0 && val <= 20) ? factorial(val) : 0.0;
    }

    for (size_t i = 0; i < arr.size(); ++i)
        tan_arr[i] = tan(arr[i]);

    double end_time = omp_get_wtime();

    cout << "\n[Sequential Results]\n";
    cout << "Time: " << end_time - start_time << " seconds\n";

    cout << "Rounded: ";
    for (const auto& x : rounded) cout << x << " ";
    cout << "\n";

    cout << "Factorial: ";
    for (const auto& x : factorial_arr) cout << x << " ";
    cout << "\n";

    cout << "Tangent: ";
    for (const auto& x : tan_arr) cout << x << " ";
    cout << "\n";
}
void parallel_processing(const vector<double>& arr, int num_threads) {
    vector<double> rounded(arr.size());
    vector<double> factorial_arr(arr.size());
    vector<double> tan_arr(arr.size());

    double start_time = omp_get_wtime();

#pragma omp parallel sections num_threads(num_threads)
    {
#pragma omp section
        for (size_t i = 0; i < arr.size(); ++i)
            rounded[i] = round(arr[i]);

#pragma omp section
        for (size_t i = 0; i < arr.size(); ++i) {
            int val = static_cast<int>(round(arr[i]));
            factorial_arr[i] = (val >= 0 && val <= 20) ? factorial(val) : 0.0;
        }

#pragma omp section
        for (size_t i = 0; i < arr.size(); ++i)
            tan_arr[i] = tan(arr[i]);
    }

    double end_time = omp_get_wtime();

    cout << "\n[Parallel Results]\n";
    cout << "Time: " << end_time - start_time << " seconds\n";

    //cout << "Rounded: ";
    //for (const auto& x : rounded) cout << x << " ";
    //cout << "\n";

    //cout << "Factorial: ";
    //for (const auto& x : factorial_arr) cout << x << " ";
    //cout << "\n";

    //cout << "Tangent: ";
    //for (const auto& x : tan_arr) cout << x << " ";
    //cout << "\n";
}
void task4() {
    srand(static_cast<unsigned>(time(nullptr)));

    int size, num_threads;
    cout << "Enter array size: ";
    cin >> size;
    cout << "Enter number of threads: ";
    cin >> num_threads;

    vector<double> arr(size);
    initialize_array(arr);

    //cout << "\nOriginal Array: ";
    //for (const auto& x : arr) cout << x << " ";
    //cout << "\n";

    sequential_processing(arr);
    parallel_processing(arr, num_threads);

    _getch();
}

void initialize_matrix(vector<vector<double>>& matrix) {
    int size = matrix.size();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            matrix[i][j] = static_cast<double>(rand()) / RAND_MAX * 100.0;
        }
    }
}
void print_matrix(const vector<vector<double>>& matrix) {
    int size = matrix.size();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << fixed << setprecision(2) << setw(8) << matrix[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}
void transform_matrix_sequential(vector<vector<double>>& matrix) {
    int size = matrix.size();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((i + j) % 2 == 0) { 
                matrix[i][j] *= 3.0;
            }
            else { 
                matrix[i][j] *= 2.0;
            }
        }
    }
}
void transform_matrix_parallel(vector<vector<double>>& matrix, int num_threads) {
    int size = matrix.size();
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if ((i + j) % 2 == 0) {  
                matrix[i][j] *= 3.0;
            }
            else { 
                matrix[i][j] *= 2.0;
            }
        }
    }
}
void task5() {
    int size, num_threads;

    cout << "Enter the size of the matrix: ";
    cin >> size;

    cout << "Enter the number of threads: ";
    cin >> num_threads;

    vector<vector<double>> matrix(size, vector<double>(size));
    srand(static_cast<unsigned int>(time(nullptr)));
    initialize_matrix(matrix);
    //cout << "Original matrix:" << endl;
    //print_matrix(matrix);
    vector<vector<double>> matrix_seq = matrix;
    vector<vector<double>> matrix_par = matrix;

    double start_time = omp_get_wtime();
    transform_matrix_sequential(matrix_seq);
    double sequential_time = omp_get_wtime() - start_time;

    start_time = omp_get_wtime();
    transform_matrix_parallel(matrix_par, num_threads);
    double parallel_time = omp_get_wtime() - start_time;

    //cout << "Sequential result:" << endl;
    //print_matrix(matrix_seq);

    //cout << "Parallel result:" << endl;
    //print_matrix(matrix_par);

    cout << "Execution times:" << endl;
    cout << "Sequential time: " << sequential_time << " s" << endl;
    cout << "Parallel time: " << parallel_time << " s" << endl;

    _getch();
}

void dop1() {
    const int size = 10;
    std::vector<int> result(size);

    result[0] = 1;

#pragma omp parallel firstprivate(result)
    {
#pragma omp for
        for (int i = 1; i < size; ++i) {
            result[i] = i * result[i - 1];
        }
    }

    std::cout << "Result array:\n";
    for (int i = 0; i < size; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    _getch();
}

vector<int> generate_random_array(int size, int min_val, int max_val) {
    vector<int> arr(size);
    srand(static_cast<unsigned>(time(nullptr)));

#pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        arr[i] = min_val + rand() % (max_val - min_val + 1);
    }

    return arr;
}
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> left_arr(n1), right_arr(n2);

    for (int i = 0; i < n1; i++)
        left_arr[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        right_arr[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[k] = left_arr[i];
            i++;
        }
        else {
            arr[k] = right_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = right_arr[j];
        j++;
        k++;
    }
}
void merge_sort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

#pragma omp parallel sections
        {
#pragma omp section
            merge_sort(arr, left, mid);

#pragma omp section
            merge_sort(arr, mid + 1, right);
        }
        merge(arr, left, mid, right);
    }
}
void dop2() {
    int array_size = 6;
    int min_value = -20;
    int max_value = 20;

    //vector<int> arr(array_size);
    //cout << "Enter " << array_size << " elements: " << endl;
    //for (int i = 0; i < array_size; i++) {
    //    cout << "arr[" << i << "]: ";
    //    cin >> arr[i];
    //}
    vector<int> arr = generate_random_array(array_size, min_value, max_value);

    cout << "The original array:" << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    merge_sort(arr, 0, arr.size() - 1);

    cout << "Sorted array:" << endl;
    for (int num : arr) {
        cout << num << " ";
    }
    cout << endl;

    _getch();
}

void process_image_negative(unsigned __int8* image, int width, int height, int num_threads) {
#pragma omp parallel for num_threads(num_threads)
    for (int i = 0; i < width * height; i++) {
        image[i] = 255 - image[i];
    }
}
void dop3() {
    const char* input_path = "peppers.bmp";
    const char* output_path = "peppers_negative.bmp";
    const int num_threads = omp_get_max_threads();

    BmpFileHeader file_header;
    BmpInfoHeader info_header;
    unsigned __int8 color_map[1024] = { 0 };

    unsigned __int8* image_data = imread((char*)input_path, info_header, file_header, color_map);

    cout << "Image size: " << info_header.width << "x" << info_header.height << endl;
    cout << "Processing with " << num_threads << " threads..." << endl;

    process_image_negative(image_data, info_header.width, info_header.height, num_threads);

    imwrite((char*)output_path, image_data, info_header, file_header, color_map);

    cout << "Negative image saved to: " << output_path << endl;

    delete[] image_data;
    _getch();
}

int main() {
    int task = 0;
    while (task != 9) {
        system("cls");
        cout << "\n\tChoose task:" << "\n\tTask 1 - 1" << "\n\tTask 2 - 2" << "\n\tTask 3 - 3" << "\n\tTask 4 - 4" << "\n\tTask 5 - 5" << "\n\tDop task 1 - 6" << "\n\tDop task 2 - 7" << "\n\tDop task 3 - 8" << "\n\tExit - 9\n";
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
            task5();
            break;
        case 6:
            dop1();
            break;
        case 7:
            dop2();
            break;
        case 8:
            dop3();
            break;
        }
    }
    system("pause");
}
