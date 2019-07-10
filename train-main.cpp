#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "Utils.hpp"
#include <unistd.h>


void print_menu();

int main() {
  Utils vu;
  int opt;
  double accuracy;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=1; i<15; i++){
            for (int j=1; j<81; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file, 0);
            }
          }
          file.close();
          break;
      }
      case 2:{
        std::cout << "Bloccato!" << '\n';
          //train("../dataset/feature_mauro", "../dataset/dataset_new.model",0,20);
          //std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
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
          for (int i=1; i<15; i++){
            for (int j=1; j<101; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file, 0);
            }
          }
          file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        std::cout << "\nGesture prediction con il file generato al puno 1. Accuracy: " << accuracy << "%\n";
        //vu.print_vec_vec(labels_probabilities, false);
        //vu.print_vec_vec(vu.order_indexes(), true);
        break;
      }
      case 5:{
        std::vector<std::vector<double>> labels_probabilities;
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105, 118, 112, 100};
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, vu.random_index(81, n_immagini[i]), file, 0);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        vu.print_vec_vec(labels_probabilities, false);
        vu.print_vec_vec(vu.order_indexes(labels_probabilities), true);
        break;
      }
      case 6:{
        std::vector<double> v_accuracy;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
          for (int j=1; j<101; j++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, j, file, 0);
          }
        }
        file.close();
        for (double perc_add = 2; perc_add < 20; perc_add++) {
          v_accuracy.push_back(train("../dataset/feature_mauro", "../dataset/dataset_new.model",1,perc_add));
        }
        std::cout << "Vettore di accuracy: \t" << '\n';
        for (size_t i = 0; i < v_accuracy.size(); i++) {
          std::cout << v_accuracy.at(i) << '\t';
        }
        std::cout << '\n';
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
            <<"6) Test con cross validation\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}
