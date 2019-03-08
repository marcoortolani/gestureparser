#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "Utils.hpp"
#include "earley/parse_sentence.cpp"
#include "earley/EarleyParser.h"
#include "earley/Stringable.h"
#include <unistd.h>
#include <fstream>
#include <boost/tokenizer.hpp>

#define FILE_GESTI "../input/comandi_corretti_labels.txt"

void print_menu();


int main() {
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  Utils vu;
  int opt;
  double accuracy;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        std::vector<int> comando_v;
        // comando_v.push_back(2);
        // comando_v.push_back(6);
        // comando_v.push_back(10);
        // comando_v.push_back(2);
        // comando_v.push_back(1);
        std::string input;
        std::cout << "Inserisci comando da parsare." << '\n';
        bool error =true;
        while (error) {
          comando_v.clear();
          error = false;
          while (std::cin >> input && input != ";") {
            comando_v.push_back(ig.word_to_label(input));
            if (ig.word_to_label(input)==-1){
              std::cout << "ERROR: rewrite the command!" << '\n';
              error=true;
            }
          }
          comando_v.push_back(1);
        }
        std::cout << "Comando digitato:" << '\n';
        vu.stampa_comando(comando_v);
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=0; i<(int)comando_v.size(); i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(comando_v.at(i), vu.random_index(91, n_immagini[comando_v.at(i)]), file);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
        vu.print_vec_vec(labels_probabilities, false);
        ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM, true);
        break;
      }
      case 2:{
        std::vector<std::string> sentence_parser;
        std::vector<std::string> sentence_svm_parser;
        sentence_parser=parse_sentence(vu.generic_label_probs(), true);
        sentence_svm_parser=parse_sentence(vu.remove_label(labels_probabilities), false);
        std::cout << "Frase riconosciuta dal solo parser" << '\n';
        if (sentence_parser.size()!=0){
          for (size_t i = 0; i < sentence_parser.size(); i++) {
            std::cout << sentence_parser.at(i) << ' ';
          }
          std::cout << "\n\n";
        } else std::cout << "Il parser non ha riconosciuto la frase" << "\n\n";
        std::cout << "Frase riconosciuta dalla combinazione SVM + parser" << '\n';
        if (sentence_svm_parser.size()!=0){
          for (size_t i = 0; i < sentence_svm_parser.size(); i++) {
            std::cout << sentence_svm_parser.at(i);
          }
          std::cout << "\n\n";
        } else std::cout << "La combinazione Parser+SVM non ha riconosciuto la frase" << "\n\n";
        break;
      }
      case 3:{
        int diverse_SVMeParser_orig=0;
        int diverse_SVMeParser_parser=0;
        int diverse_parser_orig=0;
        int uguali_SVMeParser_orig=0;
        int uguali_SVMeParser_parser=0;
        int uguali_parser_orig=0;
        int non_riconosciuto_svm_parser=0;
        int non_riconosciuto_parser=0;
        std::vector<std::string> sentence_parser;
        std::vector<std::string> sentence_svm_parser;
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        std::vector<std::vector<int>> comandi = vu.read_commands(FILE_GESTI);
        std::vector<int> comando;
        size_t zzz;
        for (zzz = 0; zzz < 5; zzz++) {
        for (int i = 0; i < (int) comandi.size(); i++) {
          comando=comandi.at(i);
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int j=0; j<(int)comando.size(); j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(comando.at(j), vu.random_index(91, n_immagini[comando.at(j)]), file);
          }
          file.close();
          labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr", accuracy);
          ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM, false);
          sentence_parser.clear();
          std::cout.setstate(std::ios_base::failbit);
          sentence_parser=parse_sentence(vu.generic_label_probs(), true);
          sentence_svm_parser=parse_sentence(vu.remove_label(labels_probabilities), false);          std::cout.clear();
          std::cout << "Original Sentence: " << '\n';
          vu.stampa_comando(comando);
          std::cout << "Frase riconosciuta dal solo parser" << '\n';
          if (sentence_parser.size()!=0){
            for (size_t i = 0; i < sentence_parser.size(); i++) {
              std::cout << sentence_parser.at(i) << ' ';
            }
            std::cout << "\n";
          } else{
            non_riconosciuto_parser++;
            std::cout << "Il parser non ha riconosciuto la frase" << "\n";
          }
          std::cout << "Frase riconosciuta dalla combinazione SVM + parser" << '\n';
          if (sentence_svm_parser.size()!=0){
            for (size_t i = 0; i < sentence_svm_parser.size(); i++) {
              std::cout << sentence_svm_parser.at(i);
            }
            std::cout << "\n\n";
          } else{
            non_riconosciuto_svm_parser++;
            std::cout << "La combinazione Parser+SVM non ha riconosciuto la frase" << "\n";
          }
          std::vector<std::string> comando_vector;
          for (size_t i = 0; i < comando.size(); i++) {
            comando_vector.push_back(ig.parse_label(comando.at(i), i));
          }
          if(ig.test_sentences_equal(sentence_svm_parser,comando_vector)) { uguali_SVMeParser_orig++; } else { diverse_SVMeParser_orig++; }
          if(ig.test_sentences_equal(sentence_svm_parser,sentence_parser)) { uguali_SVMeParser_parser++; } else { diverse_SVMeParser_parser++; }
          if(ig.test_sentences_equal(sentence_parser, comando_vector)) { uguali_parser_orig++; } else { diverse_parser_orig++; }
        }
      }
        std::cout << "Comandi totali: " << comandi.size()*zzz <<'\n';
        std::cout << "Comandi SVM+Parser diversi dai comandi corretti: " << diverse_SVMeParser_orig<< '\n';
        std::cout << "Comandi Parser diversi dai comandi corretti: " << diverse_parser_orig<< "\n\n";
        std::cout << "Comandi SVM+Parser diverse dai comandi Parser: " << diverse_SVMeParser_parser<< '\n';
        std::cout << "Comandi SVM+Parser uguali ai comandi Parser: " << uguali_SVMeParser_parser<< "\n\n";
        std::cout << "Comandi SVM+Parser uguali ai comandi corretti: " << uguali_SVMeParser_orig<< '\n';
        std::cout << "Comandi Parser uguali ai comandi corretti: " << uguali_parser_orig<< "\n\n";
        std::cout << "Comandi non riconosciuti dal Parser: " << non_riconosciuto_parser << '\n';
        std::cout << "Comandi non riconosciuti da SVM+Parser: " << non_riconosciuto_svm_parser << "\n\n";
        if((uguali_SVMeParser_orig-uguali_parser_orig)>0){
          std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_SVMeParser_orig-uguali_parser_orig << " comandi in più rispetto al solo parser" << '\n';
        } else {
          std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_parser_orig-uguali_SVMeParser_orig << " comandi in meno rispetto al solo parser" << '\n';
        }
        cout << '\a';
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
            <<"2) Passa al parser il comando\n"
            <<"3) Simulazione\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}
