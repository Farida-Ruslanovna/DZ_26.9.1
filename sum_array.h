#ifndef _SUMARRAYH_
#define _SUMARRAYH_

#include <vector>

class SumArray
{
private:
    int size;                   //size of vector
    int num_threads;            //number of threads to calculate sum
    std::vector<int>values;     //vector of values
    int result;                 //result is a sum of all values of vector
    void calc_sum_part(int l_index, int r_index, int index);    //calculate a sum of values of the interval [l_index, r_index]
public:
    SumArray(int _size, int _num_threads);          //_size - size of vector, _num_threads - nummber of threads
    ~SumArray();
    int calculate_using_threads();                  //calculate sum of values using threads
    int calculate_without_threads();                //calculate sum of values in main thread
    void printArray();                              //print values of vector
};

#endif