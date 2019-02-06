#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "VectorsUtils.hpp"
#include "pcfg/earley-parser.cpp"
#include "pcfg/earley/EarleyParser.h"
#include "pcfg/earley/Stringable.h"
#include <unistd.h>


void print_menu();

int main() {
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  VectorsUtils vu;
  int opt;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        int comando[]={1, 3, 8, 1};
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=0; i<4; i++){
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
        // std::vector<std::vector<double>> svm_predictions_no_label;
        // std::vector<double> temp;
        // for (size_t i = 0; i < 11; i++) {
        //   temp.clear();
        //   for (size_t j = 0; j < 12; j++) {
        //     temp.push_back(labels_probabilities.at(i).at(j+1));
        //   }
        //   svm_predictions_no_label.push_back(temp);
        // }
        // parser_di_earley(svm_predictions_no_label);
        parser_di_earley(labels_probabilities);
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
