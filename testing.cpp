#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "Utils.hpp"
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "earley/parse_sentence.cpp"
#include "earley/EarleyParser.h"
#include "earley/Stringable.h"
#define FILE_GESTI "../input/comandi_corretti_labels.txt"

double train_testing(int in, std::vector<std::vector<int>> &training, std::vector<std::vector<int>> &testing);
double parser(std::vector<std::vector<int>> testing);

int main(int argc, char *argv[]) {
  std::ofstream file_stats;
  std::vector<double> accuracy;
  std::vector<int> miglioramenti;
  double mean_accuracy;
  double media_miglioramenti;
  std::vector<std::vector<int>> training, testing;
  int in=10;
  while (in<11) {
    std::cout << "Quante features uso per addestrare il modello?" << '\n';
    std::cin >> in;
    std::cout << "Utilizzerò " << in << " features per addestrare il modello e circa " << 100-in << " per testarlo.\n";
    for (int exec = 0; exec < (int) 100/in; exec++) {
      std::cout << "Esecuzione " << in << "."<< exec <<'\n';
      double current_accuracy=train_testing(in, training, testing);
      accuracy.push_back(current_accuracy);
      mean_accuracy=mean_accuracy+current_accuracy;
      int current_miglioramenti = parser(testing);
      miglioramenti.push_back(current_miglioramenti);
      media_miglioramenti=media_miglioramenti+current_miglioramenti;
    }
    mean_accuracy=mean_accuracy/((int) 100/in);
    media_miglioramenti=media_miglioramenti/((int) 100/in);
    file_stats.open("../dataset/statistiche", std::ios_base::app);
    file_stats << "Addestrato con " << in << " features. \nAccuracy media SVM: " << mean_accuracy << " \nMiglioramenti medi SVM+parser: " << ceil(media_miglioramenti) << "\n";
    file_stats << "Vettore accuracy:\t";
    for (size_t i = 0; i < accuracy.size(); i++) {
      file_stats << accuracy.at(i) << "\t";
    }
    file_stats << "\nVettore miglioramenti:\t";
    for (size_t i = 0; i < miglioramenti.size(); i++) {
      file_stats << miglioramenti.at(i) << "\t";
    }
    file_stats <<"\n\n";
    file_stats.close();
    std::cout << "Accuracy media: " <<mean_accuracy<< "%\t";
    std::cout << "Miglioramenti medi SVM+parser: " << ceil(media_miglioramenti) << "\n";
    in=in+10;
  }
  return 0;
}

double train_testing(int in, std::vector<std::vector<int>> &training, std::vector<std::vector<int>> &testing){
  Utils vu;
  FeaturesExtraction* featureextr;
  training.clear();
  testing.clear();
    vu.generateImagesVector(in, training, testing);
    std::ofstream file;
    int num_feat=0;
    file.open("../dataset/feature_mauro");
    for (int i=0; i<11; i++){
      for (int j=0; j<(int)training.at(i).size(); j++){
        featureextr= new FeaturesExtraction();
        featureextr->genFeatures(i+1, training.at(i).at(j), file);
        delete featureextr;
        num_feat++;
      }
    }
    file.close();
    //std::cout << "Before training" << '\n';
    //std::cin.ignore();
    //std::cin.get();
    train("../dataset/feature_mauro", "../dataset/dataset_testing.model",0,20);
    std::cout << "Addestrato con " <<num_feat<<" features."<< '\n';
    num_feat=0;
    file.open("../dataset/feature_mauro");
    for (int i=0; i<11; i++){
      for (int j=0; j<(int)testing.at(i).size(); j++){
        featureextr= new FeaturesExtraction();
        featureextr->genFeatures(i+1, testing.at(i).at(j), file);
        delete featureextr;
        num_feat++;
      }
    }
    file.close();
    //std::cout << "Before prediction" << '\n';
    //std::cin.ignore();
    //std::cin.get();
    double current_accuracy;
    gesture_prediction("../dataset/feature_mauro","../dataset/dataset_testing.model","../dataset/prob.khr",current_accuracy);
    std::cout << "Testato con " << num_feat<<" features." << '\n';
    return current_accuracy;
}

double parser(std::vector<std::vector<int>> testing){
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  Utils vu;
  double accuracy;
  int uguali_SVMeParser_orig=0;
  int uguali_parser_orig=0;
  int diverse_SVMeParser_orig=0;
  int diverse_parser_orig=0;
  std::vector<std::string> sentence_parser;
  std::vector<std::string> sentence_svm_parser;
  std::vector<std::vector<int>> comandi = vu.read_commands(FILE_GESTI);
  std::vector<int> comando;
  for (int i = 0; i < (int) comandi.size(); i++) {
    comando=comandi.at(i);
    std::ofstream file;
    file.open("../dataset/feature_mauro");
    for (int j=0; j<(int)comando.size(); j++){
        FeaturesExtraction featureextr;
        featureextr.genFeatures(comando.at(j), testing.at(comando.at(j)-1).at(vu.random_index(1, testing.size())), file);
    }
    file.close();
    labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_testing.model","../dataset/prob.khr", accuracy);
    ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM);
    sentence_parser.clear();
    std::cout.setstate(std::ios_base::failbit);
    sentence_parser=parse_sentence(vu.generic_label_probs(), true);
    sentence_svm_parser=parse_sentence(vu.remove_label(labels_probabilities), false);          std::cout.clear();
    //std::cout << "Original Sentence: " << '\n';
    //vu.stampa_comando(comando);
    //std::cout << "Frase riconosciuta dal solo parser" << '\n';
    // if (sentence_parser.size()!=0){
    //   for (size_t i = 0; i < sentence_parser.size(); i++) {
    //     std::cout << sentence_parser.at(i) << ' ';
    //   }
    //   std::cout << "\n";
    // } else{
    //   std::cout << "Il parser non ha riconosciuto la frase" << "\n";
    // }
    // std::cout << "Frase riconosciuta dalla combinazione SVM + parser" << '\n';
    // if (sentence_svm_parser.size()!=0){
    //   for (size_t i = 0; i < sentence_svm_parser.size(); i++) {
    //     std::cout << sentence_svm_parser.at(i);
    //   }
    //   std::cout << "\n\n";
    // } else{
    //   std::cout << "La combinazione Parser+SVM non ha riconosciuto la frase" << "\n";
    // }
    std::vector<std::string> comando_vector;
    for (size_t i = 0; i < comando.size(); i++) {
      comando_vector.push_back(ig.parse_label(comando.at(i), i));
    }
    if(ig.test_sentences_equal(sentence_parser, comando_vector)) { uguali_parser_orig++; } else { diverse_parser_orig++; }
    if(ig.test_sentences_equal(sentence_svm_parser,comando_vector)) { uguali_SVMeParser_orig++; } else { diverse_SVMeParser_orig++; }
  }
  if((uguali_SVMeParser_orig-uguali_parser_orig)>0){
    std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_SVMeParser_orig-uguali_parser_orig << " comandi in più rispetto al solo parser" << '\n';
  } else {
    std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_parser_orig-uguali_SVMeParser_orig << " comandi in meno rispetto al solo parser" << '\n';
  }
  return uguali_SVMeParser_orig-uguali_parser_orig;
}