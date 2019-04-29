#include "hand-recognizer.h"
#include "train.h"
#include "FeaturesExtraction.hpp"
#include "Utils.hpp"
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <sys/time.h>
#include "earley/parse_sentence.cpp"
#include "earley/EarleyParser.h"
#include "earley/Stringable.h"
#define FILE_GESTI "../input/comandi_corretti_labels.txt"

double train_testing(int in, std::vector<std::vector<int>> &indexes, int seed);
std::vector<int> parser(int in, std::vector<std::vector<int>> indexes);

int main(int argc, char *argv[]) {
  int in, max, num_exec, seed;
  bool script;
  std::string output_filename;
  if(argc > 1 && argc<5){
      std::cout << "Modalità script.\nUsage:" << std::endl;
      std::cout << "./testing <n_features_addestramento> <cicli_per_ogni_addestramento> <file_output> <seme>" << std::endl;
      exit(1);
  } else if(argc > 1 && argc==5){
    std::cout << "Modalità script.\n";
    in= (int) std::atoi(argv[1]);
    max=in+1;
    num_exec=(int) std::atoi(argv[2]);
    std::string temp_filename(argv[3]);
    output_filename=temp_filename;
    seed=(int) std::atoi(argv[4]);
    script=true;
  } else {
    std::cout << "Modalità no script." << '\n';
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    char buffer [120];
    strftime (buffer,120,"../risultati/test_no_script_%Y_%m_%d-%H_%M",now);
    in=5;
    max=81;
    num_exec=10;
    script=false;
    output_filename=buffer;
    std::cout << "output_filename: " << output_filename << '\n';
  }
  std::string output_filename_csv = output_filename;
  output_filename_csv.append(".csv");
  std::cout << "output_filename_csv: " << output_filename_csv << '\n';
  timeval start, stop;
  double elapsedTime;
  std::ofstream file_stats;
  std::ofstream file_stats_csv;
  std::vector<double> accuracy;
  std::vector<int> miglioramenti;
  std::vector<int> non_riconosciute;
  std::vector<double> accuracy_tot;
  std::vector<int> miglioramenti_tot;
  std::vector<int> non_riconosciute_tot;
  std::vector<int> temp_stats;
  double mean_accuracy;
  double miglioramenti_media;
  double non_riconosciute_media;
  std::vector<std::vector<int>> indexes;
  gettimeofday(&start, NULL);
  while (in<max) {
    accuracy.clear();
    miglioramenti.clear();
    non_riconosciute.clear();
    mean_accuracy=0;
    miglioramenti_media=0;
    non_riconosciute_media=0;
    std::cout << "Utilizzerò " << in << "*14 features per addestrare il modello e circa " << 100-in << "*14 per testarlo.\n";
    file_stats_csv.open(output_filename_csv, std::ios_base::app);
    if (!script) {seed=(int)std::time(0);}
    file_stats_csv << in << " features;"<<"\n";
    for (int exec = 0; exec < num_exec; exec++) {
      temp_stats.clear();
      std::cout << "Esecuzione " << in << "-"<< exec+1 <<'\n';
      double current_accuracy=train_testing(in, indexes, seed);
      accuracy.push_back(current_accuracy);
      mean_accuracy=mean_accuracy+current_accuracy;
      temp_stats = parser(in, indexes);
      miglioramenti.push_back(temp_stats.at(0));
      non_riconosciute.push_back(-temp_stats.at(1));
      miglioramenti_media=miglioramenti_media+temp_stats.at(0);
      non_riconosciute_media=non_riconosciute_media+temp_stats.at(1);
    }
    mean_accuracy=mean_accuracy/num_exec;
    miglioramenti_media=miglioramenti_media/num_exec;
    non_riconosciute_media=non_riconosciute_media/num_exec;
    file_stats.open(output_filename, std::ios_base::app);
    file_stats << "Addestrato con " << in << " features. \nAccuracy media SVM: " << mean_accuracy << " \nMiglioramenti SVM+parser: " << ceil(abs(miglioramenti_media)) << "\nnon_riconosciute_media  SVM+parser: " << ceil(abs(non_riconosciute_media))<<"\n";
    file_stats_csv << "Accuracy;";
    file_stats << "Vettore accuracy:\t";
    for (size_t i = 0; i < accuracy.size(); i++) {
      file_stats << accuracy.at(i) << "\t";
      file_stats_csv <<accuracy.at(i) << ";";
    }
    file_stats_csv<<";Media:;"<<mean_accuracy<<";\nMiglioramenti:;";
    file_stats << "\nVettore miglioramenti:\t";
    for (size_t i = 0; i < miglioramenti.size(); i++) {
      file_stats << miglioramenti.at(i) << "\t";
      file_stats_csv << miglioramenti.at(i) << ";";
    }
    file_stats_csv<<";Media:;;"<<ceil(abs(miglioramenti_media))<<";\nNon Riconosciuti:;";
    file_stats << "\nVettore non_riconosciute:\t";
    for (size_t i = 0; i < non_riconosciute.size(); i++) {
      file_stats << non_riconosciute.at(i) << "\t";
      file_stats_csv <<non_riconosciute.at(i) << ";";
    }
    file_stats <<"\n\n";
    file_stats.close();
    file_stats_csv<<";Media:;;;"<<ceil(abs(non_riconosciute_media))<<";\n;\n;\n";
    file_stats_csv.close();
    accuracy_tot.push_back(mean_accuracy);
    miglioramenti_tot.push_back(ceil(abs(miglioramenti_media)));
    non_riconosciute_tot.push_back(ceil(abs(non_riconosciute_media)));
    std::cout<< "Addestrato con " << in << " features. \nAccuracy media SVM: " << mean_accuracy << " \nMiglioramenti SVM+parser: " << ceil(abs(miglioramenti_media)) << "\nnon_riconosciute_media SVM+parser: " << ceil(abs(non_riconosciute_media))<< "\n\n";
    in=in+5;
    if(!script) sleep(20);
  }
  if(!script){
    file_stats.open(output_filename, std::ios_base::app);
    file_stats_csv.open(output_filename_csv, std::ios_base::app);
    std::cout << "Riepilogo accuracy: ";
    file_stats << "Riepilogo accuracy: ";
    file_stats_csv << "Riepilogo accuracy:;";
    for (size_t i = 0; i < accuracy_tot.size(); i++) {
    std::cout << accuracy_tot.at(i)<< " ";
    file_stats << accuracy_tot.at(i)<< " ";
    file_stats_csv<< accuracy_tot.at(i)<<";";
    }
    std::cout << "\nRiepilogo miglioramenti: ";
    file_stats << "\nRiepilogo miglioramenti: ";
    file_stats_csv << "\nRiepilogo miglioramenti:;";
    for (size_t i = 0; i < miglioramenti_tot.size(); i++) {
    std::cout << miglioramenti_tot.at(i)<< " ";
    file_stats << miglioramenti_tot.at(i)<< " ";
    file_stats_csv << miglioramenti_tot.at(i)<< ";";
    }
    std::cout << "\nRiepilogo non_riconosciute: ";
    file_stats << "\nRiepilogo non_riconosciute: ";
    file_stats_csv << "\nRiepilogo non_riconosciute:;";
    for (size_t i = 0; i < non_riconosciute_tot.size(); i++) {
    std::cout << non_riconosciute_tot.at(i)<< " ";
    file_stats << non_riconosciute_tot.at(i)<< " ";
    file_stats_csv << non_riconosciute_tot.at(i)<< ";";
    }
    std::cout << '\n';
    file_stats << '\n';
    file_stats_csv << '\n';
    gettimeofday(&stop, NULL);
    elapsedTime = (stop.tv_sec - start.tv_sec);
    std::cout << "Tempo trascorso: " << (int) elapsedTime/60 << " minuti circa!\n";
    file_stats << "Tempo trascorso: " << (int) elapsedTime/60 << " minuti circa!\n\nFINE TEST\n\n";
    file_stats.close();
    file_stats_csv << "Tempo trascorso (minuti):;" << (int) elapsedTime/60 << ";";
    file_stats_csv.close();
    sleep(15);
  }
  std::cout << '\a';
  return 0;
}

double train_testing(int in, std::vector<std::vector<int>> &indexes, int seed){
  Utils vu;
  FeaturesExtraction* featureextr;
  indexes.clear();
    indexes=vu.generateIndexesVector(seed);
    std::ofstream file;
    int num_feat=0;
    file.open("../dataset/feature_mauro");
    for (int i=0; i<14; i++){
      for (int j=0; j<in; j++){
        featureextr= new FeaturesExtraction();
        featureextr->genFeatures(i+1, indexes.at(i).at(j), file);
        delete featureextr;
        num_feat++;
      }
    }
    file.close();
    train("../dataset/feature_mauro", "../dataset/dataset_testing.model",0,20);
    std::cout << "Addestrato con " <<num_feat<<" features."<< '\n';
    num_feat=0;
    file.open("../dataset/feature_mauro");
    for (int i=0; i<14; i++){
      for (int j=in; j<(int)indexes.at(i).size(); j++){
        featureextr= new FeaturesExtraction();
        featureextr->genFeatures(i+1, indexes.at(i).at(j), file);
        delete featureextr;
        num_feat++;
      }
    }
    file.close();
    double current_accuracy;
    gesture_prediction("../dataset/feature_mauro","../dataset/dataset_testing.model","../dataset/prob.khr",current_accuracy);
    std::cout << "Testato con " << num_feat<<" features." << '\n';
    return current_accuracy;
}

std::vector<int> parser(int in, std::vector<std::vector<int>> indexes){
  std::vector<std::vector<double>> labels_probabilities;
  InputGenerator ig;
  Utils vu;
  double accuracy;
  int uguali_SVMeParser_orig=0;
  int uguali_parser_orig=0;
  int non_riconosciuto_svm_parser=0;
  int non_riconosciuto_parser=0;
  std::vector<std::string> sentence_parser;
  std::vector<std::string> sentence_svm_parser;
  std::vector<std::vector<int>> comandi = vu.read_commands(FILE_GESTI);
  std::vector<int> comando;
  for (int i = 0; i < (int) comandi.size(); i++) {
    std::cout << "Comando: " << i << '\r';
    comando=comandi.at(i);
    std::ofstream file;
    file.open("../dataset/feature_mauro");
    for (int j=0; j<(int)comando.size(); j++){
        FeaturesExtraction featureextr;
        featureextr.genFeatures(comando.at(j), vu.random_index(in, indexes.at(comando.at(j)-1).size()), file);
    }
    file.close();
    labels_probabilities=gesture_prediction("../dataset/feature_mauro","../dataset/dataset_testing.model","../dataset/prob.khr", accuracy);
    ig.parse_svm_output(OUTPUT_PROBABILISTIC_SVM, false);
    sentence_parser.clear();
    std::cout.setstate(std::ios_base::failbit);
    sentence_parser=parse_sentence(vu.generic_label_probs(), true);
    sentence_svm_parser=parse_sentence(vu.remove_label(labels_probabilities), false);          std::cout.clear();
    std::vector<std::string> comando_vector;
    for (size_t i = 0; i < comando.size(); i++) {
      comando_vector.push_back(ig.parse_label(comando.at(i), i));
    }
    if(ig.test_sentences_equal(sentence_svm_parser,comando_vector)) { uguali_SVMeParser_orig++; }
    if(ig.test_sentences_equal(sentence_parser, comando_vector)) { uguali_parser_orig++; }
    if (sentence_svm_parser.size()==0) {non_riconosciuto_svm_parser++;}
    if (sentence_parser.size()==0) {non_riconosciuto_parser++;}
  }
  if((uguali_SVMeParser_orig-uguali_parser_orig)>0){
    std::cout << "La combinazaione SVM+Parser ha riconosciuto " << uguali_SVMeParser_orig-uguali_parser_orig << " comandi in più rispetto al solo parser" << '\n';
  } else if ((uguali_SVMeParser_orig-uguali_parser_orig)<0){
    std::cout << "La combinazaione SVM+Parser ha riconosciuto " << -uguali_SVMeParser_orig+uguali_parser_orig << " comandi in meno rispetto al solo parser" << '\n';
  } else {
    std::cout << "La combinazaione SVM+Parser ha riconosciuto lo stesso numero di comandi del solo parser" << '\n';
  }
  if((non_riconosciuto_svm_parser-non_riconosciuto_parser)>0){
    std::cout << "La combinazaione SVM+Parser ha saputo parsare " << uguali_SVMeParser_orig-uguali_parser_orig << " comandi in meno rispetto al parser" << '\n';
  } else if ((non_riconosciuto_svm_parser-non_riconosciuto_parser)<0){
    std::cout << "La combinazaione SVM+Parser ha saputo parsare " << -non_riconosciuto_svm_parser+non_riconosciuto_parser << " comandi in più rispetto al parser." << '\n';
  } else {
    std::cout << "La combinazaione SVM+Parser e il solo parser non hanno saputo parsare lo stesso numero di comandi: " << non_riconosciuto_svm_parser << '\n';
  }
  std::cout << '\n';
  std::vector<int> v;
  v.push_back(uguali_SVMeParser_orig-uguali_parser_orig);
  v.push_back(non_riconosciuto_svm_parser-non_riconosciuto_parser);
  return v;
}
