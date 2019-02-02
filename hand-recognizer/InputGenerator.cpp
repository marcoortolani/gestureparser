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

// void InputGenerator::parse(const char* filename){
//     std::ofstream sentence;
//     std::string line;
//     input_file.open(filename);
//     sentence.open(INPUT_PARSER_FILE,std::ofstream::out | std::ofstream::app);
//     while (std::getline(input_file, line)){
//         char* cmd = strtok(strdup(line.c_str()), " ");
//         if(strcmp(cmd, "0") == 0){
//             sentence<<"Set ";
//             std::cout<<"Set ";
//         }else if (strcmp(cmd,"1") == 0){
//             sentence<<"light ";
//             std::cout<<"light ";
//         }else if (strcmp(cmd,"2") == 0){
//             sentence<<"decrease ";
//             std::cout<<"decrease ";
//         }else if (strcmp(cmd,"3") == 0){
//             sentence<<";";
//             std::cout<<";\n";
//         }else if (strcmp(cmd,"4") == 0){
//             sentence<<"status ";
//             std::cout<<"status ";
//         }else if (strcmp(cmd,"5") == 0){
//             sentence<<"Query ";
//             std::cout<<"Query ";
//         }
//     }
//     sentence.close();
//     input_file.close();
// }

std::string InputGenerator::parse_label (int label, int num){
  switch (label) {
    case 1:
      if (num>1) {
        return "; ";
      } else return "Query ";
      break;
    case 2:
    if (num>1) {
      return "increase ";
    } else return "Set ";
    break;
    case 3:
    if (num>2) {
      return "decrease ";
    } else return "start ";
    case 4: return "stop "; break;
    case 5: return "heater "; break;
    case 6: return "light "; break;
    case 7: return "energymeter "; break;
    case 8: return "status "; break;
    case 9: return "med "; break;
    case 10: return "high "; break;
    case 11: return "low "; break;
  }
}

void InputGenerator::RemedialLikehoodController(const char* PProbability, const char* SVMProbability){
    int index;
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
