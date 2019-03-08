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
    void parse_svm_output(const char* filename, bool print_sentence);
    std::string parse_label (int label, int num);
    bool test_sentences_equal (std::vector<std::string> sentence1, std::vector<std::string> sentence2);
    int word_to_label(std::string cmd);
private:
    std::ifstream input_file;
};


#endif /* InputGenerator_hpp */
