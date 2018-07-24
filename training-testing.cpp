#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstdlib>



std::vector<int> permutateIndexes() {
  std::srand ( unsigned ( std::time(0) ) );
  std::vector<int> myvector;
  for (int i=1; i<31; i++) myvector.push_back(i);
  std::random_shuffle ( myvector.begin(), myvector.end() );
  return myvector;
}

int main() {
  std::vector<int> indici;
  int n_addestramento;
  float perc_add;
  std::cout << "Quante features uso per addestrare? (in percentuale)" << '\n';
  std::cin>>perc_add;
  perc_add=floor((perc_add/100)*30);
  n_addestramento=(int) perc_add;
  if(n_addestramento<30){
    for (int k=0; k<5; k++){
      indici=permutateIndexes();
      std::ofstream file;
      file.open("../dataset/feature_mauro");
      for (int i=0; i<6; i++){
        for (int j=0; j<n_addestramento; j++){
          FeaturesExtraction featureextr;
          featureextr.genFeatures(i, indici.at(j), file);
        }
      }
      file.close();
      std::cout << "\nGenerate le features per addestrare il modello" << '\n';
      train("../dataset/feature_mauro", "../dataset/dataset_new.model",0);
      std::cout << "\nModello generato" << '\n';
      file.open("../dataset/feature_mauro");
      for (int i=0; i<6; i++){
        for (int j=n_addestramento; j<30; j++){
          FeaturesExtraction featureextr;
          featureextr.genFeatures(i, indici.at(j), file);
        }
      }
      file.close();
      std::cout << "Prova " << k+1 << '\n';
      gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
      sleep(0.5);
    }
  } else{
    std::cerr << "Non valido" << '\n';
  }
  return 0;
}
