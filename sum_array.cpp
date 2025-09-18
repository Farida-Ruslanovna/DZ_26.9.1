#include "sum_array.h"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>

SumArray::SumArray(int _size, int _num_threads) : size(_size), num_threads(_num_threads)
{
    const int min_value = 0;
    const int max_value = 10;
    srand(std::time(0));
    for (int i = 0; i < size; i++)
    {
        values.push_back(min_value + rand() % (max_value - min_value + 1));
    }
}

SumArray::~SumArray()
{

}

void SumArray::printArray()
{
    std::cout << "Array:" << std::endl;
    for (int i = 0; i < size; i++)
        std::cout << values[i] << " ";
    std::cout << "\n" << std::endl;
}

void SumArray::calc_sum_part(int l_index, int r_index, int index)
{
    int s = 0;
    while (l_index <= r_index)
    {
        s += values[l_index++];
    }
    result += s;
    //std::cout << "Thread id = " << std::this_thread::get_id() << "  index = " << index << " sum = " << s << std::endl;
}

int SumArray::calculate_without_threads()
{
    int s = 0;
    for (int i = 0; i < values.size(); i++)
    {
        s += values[i];
    }
    return s;
}

int SumArray::calculate_using_threads()
{
    std::vector<std::thread> threads;
    int l_index = 0;
    int r_index = 0;
    int elements_count = size / num_threads;
    result = 0;

    for (int i = 0; i < num_threads; i++)
    {
        if (i == num_threads - 1)
            r_index = values.size();    //if we have left elements, we consider them
        else
            r_index += elements_count;

        //create a thread
        std::thread t(&SumArray::calc_sum_part, this, l_index, r_index - 1, i);
        //we cannot copy a thread object so we move it
        threads.push_back(std::move(t));
        l_index += elements_count;
    }

    for (auto& t : threads)
    {
        if (t.joinable())
            t.join();
    }

    return result;
}