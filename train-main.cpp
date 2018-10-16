#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include <unistd.h>

void print_menu(){
    std::cout<<"\nOpzioni\n"
            <<"1) Genera le features dal dataset\n"
            <<"2) SVM train\n"
            <<"3) Cross Validation\n"
            <<"4) Testa il modello (col train-set)\n"
            <<"5) Testa con le immagini del vecchio dataset\n"
            <<"6) Testa con tutte le immagini del dataset\n\n"
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
          for (int i=1; i<12; i++){
            for (int j=1; j<101; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file);
            }
          }
          file.close();
          break;
      }
      case 2:{
          train("../dataset/feature_mauro", "../dataset/dataset_new.model",0,20);
          std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
        break;
      }
      case 3:{
          int perc_add;
          std::cout << "Quante features (in percentuale) uso per testare il modello?" << '\n';
          std::cin>>perc_add;
          train("../dataset/feature_mauro", "../dataset/dataset_new.model",1,perc_add);
          std::cout << "\nCross validation con il file generato al puno 1" << '\n';
        break;
      }
      case 4:{

        gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        std::cout << "\nGesture prediction con il file generato al puno 1" << '\n';
        break;
      }
      case 5:{
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=201; i<231; i++){
          FeaturesExtraction featureextr;
          featureextr.genFeatures(1, i, file);
        }
        file.close();
        gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        break;
      }
      case 6:{
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int k=1; k<12; k++){
          for (int j=1; j<5; j++){
            for (int i=1; i<31; i++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures_sporche(j, k, i, file);
            }
          }
        }
        file.close();
        gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
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
