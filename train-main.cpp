#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "VectorsUtils.hpp"
#include <unistd.h>


void print_menu();

int main() {
  VectorsUtils vu;
  int opt;
  int approx=-1;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
          std::cout << "Approssimare? \n 0) no \n 1) si" << '\n';
          std::cin >> approx;
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=1; i<12; i++){
            for (int j=1; j<91; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file, approx);
            }
          }
          file.close();
          break;
      }
      case 2:{
        std::cout << "Bloccato!" << '\n';
        /*
          train("../dataset/feature_mauro", "../dataset/dataset_new.model",0,20);
          std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
          */
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
        int approx_new;
        std::cout << "Approssimare? \n 0) no \n 1) si" << '\n';
        std::cin >> approx_new;
        if (approx_new != approx){
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int i=1; i<12; i++){
            for (int j=1; j<101; j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(i, j, file, approx_new);
            }
          }
          file.close();
        }
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        std::cout << "\nGesture prediction con il file generato al puno 1" << '\n';
        //vu.print_vec_vec(labels_probabilities, false);
        //vu.print_vec_vec(vu.order_indexes(), true);
        break;
      }
      case 5:{
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=201; i<231; i++){
          FeaturesExtraction featureextr;
          featureextr.genFeatures(1, i, file, 0);
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
      case 7:{
        std::vector<std::vector<double>> labels_probabilities;
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        std::cout << "\nSenza approssimazione\n\n";
        for (int i=1; i<12; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, vu.random_index(n_immagini[i]), file, 0);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        vu.print_vec_vec(labels_probabilities, false);
        std::cout << "\nCon approssimazione\n\n";
        for (int i=1; i<12; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, vu.random_index(n_immagini[i]), file, 1);
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
            <<"5) Testa con le immagini del vecchio dataset\n"
            <<"6) Testa con tutte le immagini del dataset\n"
            <<"7) Sorteggia immagini\n"
            <<"8) Prova\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}
