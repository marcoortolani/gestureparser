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
            command.push_back("Query ");
        }else if (strcmp(cmd,"2") == 0){
          command.push_back("Set ");
        }else if (strcmp(cmd,"3") == 0){
          command.push_back("start ");
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
        }else if (strcmp(cmd,"12") == 0){
            command.push_back("increase ");
        }else if (strcmp(cmd,"13") == 0){
            command.push_back("decrease ");
        }else if (strcmp(cmd,"14") == 0){
            command.push_back("; ");
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
    case 1: return "Query "; break;
    case 2: return "Set "; break;
    case 3: return "start "; break;
    case 4: return "stop "; break;
    case 5: return "heater "; break;
    case 6: return "light "; break;
    case 7: return "energymeter "; break;
    case 8: return "status "; break;
    case 9: return "med "; break;
    case 10: return "high "; break;
    case 11: return "low "; break;
    case 12: return "increase "; break;
    case 13: return "decrease "; break;
    case 14: return "; "; break;
  }
  return "boh ";
}

int InputGenerator::word_to_label(std::string word){
  std::transform(word.begin(), word.end(), word.begin(), ::tolower);
  word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
  const char *cmd = word.c_str();
  if(strcmp(cmd, "query") == 0) return 1;
  if(strcmp(cmd, "set") == 0) return 2;
  if(strcmp(cmd, "start") == 0) return 3;
  if(strcmp(cmd, "stop") == 0) return 4;
  if(strcmp(cmd, "heater") == 0) return 5;
  if(strcmp(cmd, "light") == 0) return 6;
  if(strcmp(cmd, "energymeter") == 0) return 7;
  if(strcmp(cmd, "status") == 0) return 8;
  if(strcmp(cmd, "med") == 0) return 9;
  if(strcmp(cmd, "high") == 0) return 10;
  if(strcmp(cmd, "low") == 0) return 11;
  if(strcmp(cmd, "increase") == 0) return 12;
  if(strcmp(cmd, "decrease") == 0) return 13;
  if(strcmp(cmd, ";") == 0) return 14;
  return -1;
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
