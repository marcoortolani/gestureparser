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
    //void parse(const char* filename);
    std::string parse_label (int label, int num);
    void RemedialLikehoodController(const char* PProbability, const char* SVMProbability);
private:
    std::ifstream input_file;
};


#endif /* InputGenerator_hpp */
