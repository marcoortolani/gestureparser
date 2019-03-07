#include "InputGenerator.hpp"

InputGenerator::InputGenerator(){

}

void InputGenerator::parse_svm_output(const char* filename, bool print_sentence){
    int num_parola=0;
    std::vector<std::string> command;
    std::ofstream sentence;
    std::string line;
    input_file.open(filename);
    sentence.open(INPUT_PARSER_FILE,std::ofstream::out | std::ofstream::trunc); //trunc o app
    while (std::getline(input_file, line)){
        char* cmd = strtok(strdup(line.c_str()), " ");
        if(strcmp(cmd, "1") == 0){
          if(num_parola>1){
            command.push_back("; ");
          }else{
            command.push_back("Query ");
          }
        }else if (strcmp(cmd,"2") == 0){
          if(num_parola>1){
            command.push_back("increase ");
          }else{
            command.push_back("Set ");
          }
        }else if (strcmp(cmd,"3") == 0){
          if(num_parola>2){
            command.push_back("decrease ");
          }else{
            command.push_back("start ");
          }
        }else if (strcmp(cmd,"4") == 0){
            command.push_back("stop ");
        }else if (strcmp(cmd,"5") == 0){
            command.push_back("heater ");
        }else if (strcmp(cmd,"6") == 0){
            command.push_back("light ");
        }else if (strcmp(cmd,"7") == 0){
            command.push_back("energymeter ");
        }else if (strcmp(cmd,"8") == 0){
            command.push_back("status ");
        }else if (strcmp(cmd,"9") == 0){
            command.push_back("med ");
        }else if (strcmp(cmd,"10") == 0){
            command.push_back("high ");
        }else if (strcmp(cmd,"11") == 0){
            command.push_back("low ");
        }
            num_parola++;
    }
    for (size_t i = 0; i < command.size()-1; i++) {
      sentence<<command.at(i);
      if (print_sentence) std::cout << command.at(i);
    }
    sentence<<";\n";
    if (print_sentence) std::cout<<";\n";
    sentence.close();
    input_file.close();
}

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
      return "decrease " ;
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
  return "boh ";
}


bool InputGenerator::test_sentences_equal (std::vector<std::string> sentence1, std::vector<std::string> sentence2){
  std::string s1, s2;
  if (sentence1.size()!=sentence2.size())  return false;
  for (int i = 0; i < (int) sentence1.size(); i++) {
    s1=sentence1.at(i);
    s2=sentence2.at(i);
    s1.erase(std::remove(s1.begin(), s1.end(), ' '), s1.end());
    s2.erase(std::remove(s2.begin(), s2.end(), ' '), s2.end());
    if (s1 != s2) {
      return false;
    }
  }
  return true;
}
