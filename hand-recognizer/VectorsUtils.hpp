#ifndef VectorsUtils_hpp
#define VectorsUtils_hpp

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <string>
#include <random>

class VectorsUtils{
  public:
    void print_vec_vec (std::vector<std::vector<double>> vec_vec, bool labels);
    std::vector<std::vector<double>> vec_vec_multip (std::vector<std::vector<double>> vec_a, std::vector<std::vector<double>> vec_b);
    std::vector<double> vec_multip (std::vector<double> vec_a, std::vector<double> vec_b);
    std::vector<std::vector<double>> order_indexes (std::vector<std::vector<double>> labels_probabilities);
    int random_index(int n);
};

#endif /*VectorsUtils_hpp*/
