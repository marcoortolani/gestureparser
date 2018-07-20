#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include <unistd.h>

void print_menu(){
    std::cout<<"\nOpzioni\n"
            <<"1) Genera le features dal dataset\n"
            <<"2) SVM train\n"
            <<"3) Cross Validation\n"
            <<"4) Testa il modello\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}

int main() {
  int opt;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=0; i<6; i++){
            for (int j=1; j<31; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file);
            }
          }
          file.close();
          break;
      }
      case 2:{
          train("../dataset/feature_mauro", "../dataset/dataset_new.model",0);
          std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
        break;
      }
      case 3:{
          train("../dataset/feature_mauro", "../dataset/dataset_new.model",1);
          std::cout << "\nCross validation con il file generato al puno 1" << '\n';
        break;
      }
      case 4:{
        gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        std::cout << "\nGesture prediction con il file generato al puno 1" << '\n';
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
