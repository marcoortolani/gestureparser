#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "VectorsUtils.hpp"
#include "pcfg/earley-parser.cpp"
#include "pcfg/earley/EarleyParser.h"
#include "pcfg/earley/Stringable.h"
#include <unistd.h>
#include <fstream>
#include <boost/tokenizer.hpp>

#define FILE_GESTI "../input/comandi_corretti_labels.txt"

void print_menu();
std::vector<std::vector<int>> read_commands(const char* filename);
void stampa_comando(std::vector<int> v);

int main() {
  std::vector<std::vector<std::string>> recognized_sentences;
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  VectorsUtils vu;
  int opt;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        int comando[]={2, 6, 10, 2, 1};;
        std::ofstream file;
        file.open("../dataset/feature_mauro");
        for (int i=0; i<5; i++){
            FeaturesExtraction featureextr;
            featureextr.genFeatures(comando[i], vu.random_index(n_immagini[comando[i]]), file, 0);
        }
        file.close();
        labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        vu.print_vec_vec(labels_probabilities, false);
        ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM);
        break;
      }
      case 2:{
        bool flag;
        parser_di_earley(labels_probabilities, flag);
        if (!flag) {
          std::cout << "Il parser non ha riconosciuto il comando\n"
                    << "quindi non è veritiero il risultato SVM+Parser\n"
                    << "Eseguire nuovamente il comando!\n\n";
        }
        break;
      }
      case 3:{
        int diverse_SVMeParser_orig=0;
        int diverse_SVMeParser_parser=0;
        int diverse_parser_orig=0;
        int uguali_SVMeParser_orig=0;
        int uguali_SVMeParser_parser=0;
        int uguali_parser_orig=0;
        int non_riconosciuti=0;
        int n_immagini[]={0, 118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
        std::vector<std::vector<int>> comandi = read_commands(FILE_GESTI);
        std::vector<int> comando;
        for (int i = 0; i < (int) comandi.size(); i++) {
          comando=comandi.at(i);
          std::ofstream file;
          file.open("../dataset/feature_mauro");
          for (int j=0; j<(int)comando.size(); j++){
              FeaturesExtraction featureextr;
              featureextr.genFeatures(comando.at(j), vu.random_index(n_immagini[comando.at(j)]), file, 0);
          }
          file.close();
          labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_new.model","../dataset/prob.khr");
          ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM);
          bool flag;
          recognized_sentences.clear();
          recognized_sentences=parser_di_earley(labels_probabilities, flag);
          if (!flag) {
            std::cout << "Il parser non ha riconosciuto il comando\n"
                      << "quindi non è veritiero il risultato SVM+Parser\n"
                      << "Eseguire nuovamente il comando!\n\n";
            non_riconosciuti++;
          }
          std::cout << "Original Sentence: " << '\n';
          stampa_comando(comando);
          std::vector<std::string> comando_vector;
          for (size_t i = 0; i < comando.size(); i++) {
            comando_vector.push_back(ig.parse_label(comando.at(i), i));
          }
          if(ig.test_sentences_equal(recognized_sentences.at(2),comando_vector)) { uguali_SVMeParser_orig++; } else { diverse_SVMeParser_orig++; }
          if(ig.test_sentences_equal(recognized_sentences.at(2),recognized_sentences.at(1))) { uguali_SVMeParser_parser++; } else { diverse_SVMeParser_parser++; }
          if(ig.test_sentences_equal(recognized_sentences.at(1), comando_vector)) { uguali_parser_orig++; } else { diverse_parser_orig++; }
        }
        std::cout << "Frasi SVM+Parser diverse dalle frasi corretta: " << diverse_SVMeParser_orig - non_riconosciuti<< '\n';
        std::cout << "Frasi SVM+Parser diverse dalle frasi riconosciute dal parser: " << diverse_SVMeParser_parser -non_riconosciuti << '\n';
        std::cout << "Frasi riconosciute dal parser diverse dalle frasi corrette: " << diverse_parser_orig -non_riconosciuti<< '\n';
        std::cout << "Frasi SVM+Parser uguali alle frasi corretta: " << uguali_SVMeParser_orig -non_riconosciuti << '\n';
        std::cout << "Frasi SVM+Parser uguali alle frasi riconosciute dal parser: " << uguali_SVMeParser_parser - non_riconosciuti << '\n';
        std::cout << "Frasi riconosciute dal parser uguali alle frasi corrette: " << uguali_parser_orig - non_riconosciuti << '\n';
        std::cout << "Non riconosciute: " << non_riconosciuti << '\n';
        if((uguali_SVMeParser_orig-uguali_parser_orig)>0){
          std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_SVMeParser_orig-uguali_parser_orig << " frasi in più rispetto al solo parser" << '\n';
        } else {
          std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_parser_orig-uguali_SVMeParser_orig << " frasi in meno rispetto al solo parser" << '\n';
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

std::vector<std::vector<int>> read_commands (const char* filename){
  std::ifstream input_file;
  std::string line;
  input_file.open(filename);
  std::vector<std::vector<int>> comandi;
  std::vector<int> tmp;
  while (std::getline(input_file, line) && input_file.good()) {
        boost::tokenizer<> tok(line);
        for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
          tmp.push_back(stoi(*beg));
        }
        comandi.push_back(tmp);
        tmp.clear();
  }
  input_file.close();
  std::cout << "Letti " << comandi.size() << " comandi dal file " << filename << '\n';
  std::cout << "Premere qualsiasi tasto per continuare." << '\n';
  cin.ignore();
  cin.get();
  return comandi;
}

void stampa_comando(std::vector<int> v){
  InputGenerator ig;
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << ig.parse_label(v.at(i), i) << ' ';
  }
  std::cout << '\n';
}
