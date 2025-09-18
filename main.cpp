// DZ_26.9.1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "sum_array.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono;

int main()
{
    const int array_size = 10000000;
    const int number_threads = 5;
    SumArray sum(array_size, number_threads);
    //sum.printArray();

    //calsulate sum using threads
    auto t_start = system_clock::now();
    int s = sum.calculate_using_threads();
    auto t_end = system_clock::now();
    long diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (using threads: " << number_threads << ") is " << s << "  Time (ms): " << diff << std::endl;

    //calculate sum in main thread
    t_start = system_clock::now();
    s = sum.calculate_without_threads();
    t_end = system_clock::now();
    diff = duration_cast<milliseconds>(t_end - t_start).count();
    std::cout << "SUM (main thread) is " << s << "  Time (ms): " << diff << std::endl;

    return 0;
}