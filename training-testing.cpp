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
  std::cout << "Indici riordinati: " << '\n';
  for (int i=1; i<31; i++) std::cout << myvector.at(i-1) << " ";
  std::cout << '\n';
  return myvector;
}

int main() {
  FeaturesExtraction* featureextr;
  std::vector<double> accuracy;
  std::vector<int> indici;
  int n_addestramento;
  float perc_add;
  std::cout << "Quante features per classe uso per addestrare? (in percentuale)" << '\n';
  std::cin>>perc_add;
  perc_add=floor((perc_add/100)*30);
  n_addestramento=(int) perc_add;
  std::cout << "Verranno usate " << n_addestramento << " features per addestrare il modello e " << 30-n_addestramento << " per testarlo." << '\n';
  if(n_addestramento<30){
    for (int k=0; k<5; k++){
      std::cout << "\nProva " << k+1 << '\n';
      indici=permutateIndexes();
      std::ofstream file;
      file.open("../dataset/feature_mauro");
      for (int i=0; i<6; i++){
        for (int j=0; j<n_addestramento; j++){
          featureextr= new FeaturesExtraction();
          featureextr->genFeatures(i, indici.at(j), file);
          delete featureextr;
        }
      }
      file.close();
      std::cout << "Generate le features per addestrare il modello" << '\n';
      train("../dataset/feature_mauro", "../dataset/dataset_new.model",0);
      std::cout << "Modello generato" << '\n';
      file.open("../dataset/feature_mauro");
      for (int i=0; i<6; i++){
        for (int j=n_addestramento; j<30; j++){
          featureextr= new FeaturesExtraction();
          featureextr->genFeatures(i, indici.at(j), file);
          delete featureextr;
        }
      }
      file.close();
      accuracy.push_back(gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr"));
      sleep(1);
    }
    double mean_acc=0.0;
    for (int k=0; k<5; k++){
      mean_acc=mean_acc+accuracy.at(k);
    }
    mean_acc=mean_acc/5;
    std::cout << "\nMean Accuracy: " << mean_acc <<'\n';
  } else{
    std::cerr << "Non valido" << '\n';
  }
  return 0;
}
