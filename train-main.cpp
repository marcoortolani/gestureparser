#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "VectorsUtils.hpp"
#include <unistd.h>


void print_menu();

int main() {
  VectorsUtils vu;
  int opt;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=1; i<12; i++){
            for (int j=1; j<91; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file);
            }
          }
          file.close();
          break;
      }
      case 2:{
        //std::cout << "Bloccato!" << '\n';
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
        std::vector<std::vector<double>> labels_probabilities;
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=1; i<12; i++){
            for (int j=1; j<101; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file);
            }
          }
          file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        std::cout << "\nGesture prediction con il file generato al puno 1" << '\n';
        //vu.print_vec_vec(labels_probabilities, false);
        //vu.print_vec_vec(vu.order_indexes(), true);
        break;
      }
      case 5:{
        std::vector<std::vector<double>> labels_probabilities;
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        std::cout << "\nSenza approssimazione\n\n";
        for (int i=1; i<12; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, vu.random_index(n_immagini[i]), file);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        vu.print_vec_vec(labels_probabilities, false);
        std::cout << "\nCon approssimazione\n\n";
        for (int i=1; i<12; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, vu.random_index(n_immagini[i]), file);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        vu.print_vec_vec(labels_probabilities, false);
        vu.print_vec_vec(vu.order_indexes(labels_probabilities), true);
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
            <<"1) Genera le features dal dataset\n"
            <<"2) SVM train, genera il modello\n"
            <<"3) Cross Validation\n"
            <<"4) Testa il modello\n"
            <<"5) Sorteggia immagini\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}
