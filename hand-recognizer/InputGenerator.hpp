//
//  InputGenerator.hpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 16/12/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

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
    void RemedialLikehoodController(const char* PProbability, const char* SVMProbability);
private:
    std::ifstream input_file;
};


#endif /* InputGenerator_hpp */
