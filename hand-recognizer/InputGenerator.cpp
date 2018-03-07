//
//  InputGenerator.cpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 16/12/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#include "InputGenerator.hpp"

InputGenerator::InputGenerator(){

}

void InputGenerator::parse(const char* filename){
    std::ofstream sentence;
    std::string line;
    input_file.open(filename);
    sentence.open(INPUT_PARSER_FILE,std::ofstream::out | std::ofstream::app);
    while (std::getline(input_file, line)){
        char* cmd = strtok(strdup(line.c_str()), " ");
        if(strcmp(cmd, "0") == 0){
            sentence<<"Set ";
        }else if (strcmp(cmd,"1") == 0){
            sentence<<"light ";
        }else if (strcmp(cmd,"2") == 0){
            sentence<<"decrease ";
        }else if (strcmp(cmd,"3") == 0){
            sentence<<";";
        }else if (strcmp(cmd,"4") == 0){
            sentence<<"status ";
        }else if (strcmp(cmd,"5") == 0){
            sentence<<"Query ";
        }
    }
    sentence.close();
    input_file.close();
}

void InputGenerator::RemedialLikehoodController(const char* PProbability, const char* SVMProbability){
    int index = -1;
    int tok = 0;
    int l_list = 14;
    double r[14] = {0.0};
    double parser_p[14] ={0.0};
    double svm_p[14]={0.0};
    while(tok < 4){
        for (int i = 0; i < l_list; i++) {
            r[i] = parser_p[i] * svm_p[i];
        }
        index = std::distance(r, std::max_element(r,r+14));
        tok++;
        //command.append(ListOfTerminals.getTerminal(index));
    }
}
