#ifndef Utils_hpp
#define Utils_hpp

#include <iostream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <string>
#include <random>
#include <unistd.h>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <boost/tokenizer.hpp>
#include "InputGenerator.hpp"

class Utils{
  public:
    void print_vec_vec (std::vector<std::vector<double>> vec_vec, bool labels);
    std::vector<std::vector<double>> vec_vec_multip (std::vector<std::vector<double>> vec_a, std::vector<std::vector<double>> vec_b);
    std::vector<double> vec_multip (std::vector<double> vec_a, std::vector<double> vec_b);
    std::vector<std::vector<double>> order_indexes (std::vector<std::vector<double>> labels_probabilities);
    int random_index(int min, int max);
    std::vector<std::vector<int>> read_commands (const char* filename);
    void stampa_comando(std::vector<int> v);
    std::vector<std::vector<double>> generic_label_probs();
    std::vector<std::vector<double>> remove_label(std::vector<std::vector<double>> probs);
    std::vector<int> permutateIndexes(int label);
    std::vector<std::vector<int>> generateIndexesVector();
};

#endif /*Utils_hpp*/
