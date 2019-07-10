#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "Utils.hpp"
#include <unistd.h>


void print_menu();

int main() {
  int opt;
  double accuracy;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        std::vector<std::vector<double>> labels_probabilities;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
          for (int j=81; j<101; j++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, j, file, 0);
          }
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        std::cout << "\nGesture prediction con il file generato al puno 1. Accuracy: " << accuracy << "%\n";
        std::ofstream file_stats_label, file_stats_predicted;
        file_stats_label.open("../python_accuracy_evaluation/file_accuracy_evaluation_labels_testset", std::ios_base::trunc);
        file_stats_predicted.open("../python_accuracy_evaluation/file_accuracy_evaluation_predicted_testset", std::ios_base::trunc);
        int temptemp=1;
        int temptemptemp=0;
        for (size_t i = 0; i < labels_probabilities.size(); i++) {
          temptemptemp++;
          if (temptemptemp>20){
            temptemptemp=1;
            temptemp++;
          }
          file_stats_label << temptemp << '\n';
          file_stats_predicted << labels_probabilities.at(i).at(0) << '\n';
        }
        file_stats_label.close();
        file_stats_predicted.close();
        break;
      }
      case 2:{
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
        std::ofstream file_stats_label, file_stats_predicted;
        file_stats_label.open("../python_accuracy_evaluation/file_accuracy_evaluation_labels", std::ios_base::trunc);
        file_stats_predicted.open("../python_accuracy_evaluation/file_accuracy_evaluation_predicted_dataset", std::ios_base::trunc);
        int temptemp=1;
        int temptemptemp=0;
        for (size_t i = 0; i < labels_probabilities.size(); i++) {
          temptemptemp++;
          if (temptemptemp>100){
            temptemptemp=1;
            temptemp++;
          }
          file_stats_label << temptemp << '\n';
          file_stats_predicted << labels_probabilities.at(i).at(0) << '\n';
        }
        file_stats_label.close();
        file_stats_predicted.close();
        break;
      }
      case 3:{
        std::vector<std::vector<double>> labels_probabilities;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
          for (int j=1; j<101; j++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, j, file, 1);
          }
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        std::cout << "\nGesture prediction con il file generato al puno 1. Accuracy: " << accuracy << "%\n";
        std::ofstream file_stats_label, file_stats_predicted;
        file_stats_label.open("../python_accuracy_evaluation/file_accuracy_evaluation_labels", std::ios_base::trunc);
        file_stats_predicted.open("../python_accuracy_evaluation/file_accuracy_evaluation_predicted_rotated", std::ios_base::trunc);
        int temptemp=1;
        int temptemptemp=0;
        for (size_t i = 0; i < labels_probabilities.size(); i++) {
          temptemptemp++;
          if (temptemptemp>100){
            temptemptemp=1;
            temptemp++;
          }
          file_stats_label << temptemp << '\n';
          file_stats_predicted << labels_probabilities.at(i).at(0) << '\n';
        }
        file_stats_label.close();
        file_stats_predicted.close();
        break;
      }
      case 4:{
        std::vector<std::vector<double>> labels_probabilities;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
          for (int j=1; j<101; j++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, j, file, 2);
          }
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        std::cout << "\nGesture prediction con il file generato al puno 1. Accuracy: " << accuracy << "%\n";
        std::ofstream file_stats_label, file_stats_predicted;
        file_stats_label.open("../python_accuracy_evaluation/file_accuracy_evaluation_labels", std::ios_base::trunc);
        file_stats_predicted.open("../python_accuracy_evaluation/file_accuracy_evaluation_predicted_scaled", std::ios_base::trunc);
        int temptemp=1;
        int temptemptemp=0;
        for (size_t i = 0; i < labels_probabilities.size(); i++) {
          temptemptemp++;
          if (temptemptemp>100){
            temptemptemp=1;
            temptemp++;
          }
          file_stats_label << temptemp << '\n';
          file_stats_predicted << labels_probabilities.at(i).at(0) << '\n';
        }
        file_stats_label.close();
        file_stats_predicted.close();
        break;
      }
      case 5:{
        std::vector<std::vector<double>> labels_probabilities;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=1; i<15; i++){
          for (int j=1; j<101; j++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(i, j, file, 3);
          }
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        std::cout << "\nGesture prediction con il file generato al puno 1. Accuracy: " << accuracy << "%\n";
        std::ofstream file_stats_label, file_stats_predicted;
        file_stats_label.open("../python_accuracy_evaluation/file_accuracy_evaluation_labels", std::ios_base::trunc);
        file_stats_predicted.open("../python_accuracy_evaluation/file_accuracy_evaluation_predicted_rotated_scaled", std::ios_base::trunc);
        int temptemp=1;
        int temptemptemp=0;
        for (size_t i = 0; i < labels_probabilities.size(); i++) {
          temptemptemp++;
          if (temptemptemp>100){
            temptemptemp=1;
            temptemp++;
          }
          file_stats_label << temptemp << '\n';
          file_stats_predicted << labels_probabilities.at(i).at(0) << '\n';
        }
        file_stats_label.close();
        file_stats_predicted.close();
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
    std::cout<<"\nGenera file per lo script di precision e recall usando il dataset:\n"
            <<"1) Testset\n"
            <<"2) Dataset\n"
            <<"3) Dataset rotated\n"
            <<"4) Dataset scaled\n"
            <<"5) Dataset scaled & rotated\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}
