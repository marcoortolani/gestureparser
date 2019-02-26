#ifndef InputGenerator_hpp
#define InputGenerator_hpp

#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string.h>

#define INPUT_PARSER_FILE "../input/sentence-hr.txt"

class InputGenerator{
public:
    InputGenerator();
    void parse_svm_output(const char* filename);
    std::string parse_label (int label, int num);
    bool test_sentences_equal (std::vector<std::string> sentence1, std::vector<std::string> sentence2);
private:
    std::ifstream input_file;
};


#endif /* InputGenerator_hpp */
