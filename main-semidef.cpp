#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "VectorsUtils.hpp"
#include "pcfg/earley-parser.cpp"
#include "pcfg/earley/EarleyParser.h"
#include "pcfg/earley/Stringable.h"
#include <unistd.h>
#include <fstream>

#define FILE_GESTI "comandi.txt"

void print_menu();
void leggi_file(const char* filename);
int main() {
  leggi_file(FILE_GESTI);
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  VectorsUtils vu;
  int opt;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 97, 105};
        int comando[]={2, 6, 10, 2, 1};
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=0; i<5; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(comando[i], vu.random_index(n_immagini[comando[i]]), file, 0);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        vu.print_vec_vec(labels_probabilities, false);
        ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM);
        break;
      }
      case 2:{
        bool flag;
        flag = parser_di_earley(labels_probabilities);
        if (!flag) {
          std::cout << "Il parser non ha riconosciuto il comando";
          std::cout << " quindi non è veritiero il risultato SVM+Parser" << '\n';
          std::cout << "Eseguire nuovamente il comando!" << "\n\n";
        }
        break;
      }
      case 3:{
        leggi_file(FILE_GESTI);
        break;
      }
      case 9:
          exit(1);
      default:
          std::cout<<"Invalid command.\n";
          break;
    }
  } while(1);
  return 0;
}

void print_menu(){
    std::cout<<"\nOpzioni\n"
            <<"1) Genera comando tramite SVM\n"
            <<"2) Passa al parser il comando\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}

void leggi_file (const char* filename){
  std::ifstream input_file;
  std::string line;
  input_file.open("comandi.txt");
  while (std::getline(input_file, line)) {
      std::cout << line << '\n';
  }
  input_file.close();
}
