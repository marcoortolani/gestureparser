#include "Utils.hpp"

//stampa vettori di vettori di double
void Utils::print_vec_vec (std::vector<std::vector<double>> vec_vec, bool labels){
  std::cout << "\nPredictions: \n\n";
  for (int i = 0; i < (int) vec_vec.size(); i++){
    for (int j = 0; j < (int) vec_vec.at(i).size(); j++){
        if (j==0) std::cout << "Predetta: ";
        std::cout << vec_vec.at(i).at(j)<< "\t";
        if (j==0){
          if(labels){
            std::cout << "Altre labels (in ordine): ";
          } else std::cout << "Probabilities: ";
        }
    }
    std::cout << '\n';
  }
}

//moltiplica vettori di vettori di double
std::vector<std::vector<double>> Utils::vec_vec_multip (std::vector<std::vector<double>> vec_a, std::vector<std::vector<double>> vec_b){
  std::vector<std::vector<double>> res;
  std::vector<double> temp_mult;
  for (int i = 0; i < (int) vec_a.size(); i++){
    temp_mult.clear();
    for (int j = 0; j < (int) vec_a.at(i).size(); j++){
        temp_mult.push_back( vec_a.at(i).at(j) * vec_b.at(i).at(j));
        }
        res.push_back(temp_mult);
    }
  return res;
}

std::vector<double> Utils::vec_multip (std::vector<double> vec_a, std::vector<double> vec_b){
  std::vector<double> res;
  if (vec_a.size() != vec_b.size()){
    std::cerr << "I vettori sono di dimensione diversa" << '\n';
    std::exit(EXIT_FAILURE);
  }
  for (int i = 0; i < (int) vec_a.size(); i++){
        res.push_back( vec_a.at(i) * vec_b.at(i));
        }
  return res;
}

//ordina gli indici in ordine di probabilità decrescente
std::vector<std::vector<double>> Utils::order_indexes (std::vector<std::vector<double>> labels_probabilities){
  std::vector<std::vector<double>> indexes;
  std::vector<double> temp_probs;
  std::vector<double> temp_indexes;
  double temp_max;
  int temp_index;
  for (int i=0; i< (int) labels_probabilities.size(); i++){
    temp_probs=labels_probabilities.at(i);
    temp_indexes.clear();
    for (int j = 1; j < (int) temp_probs.size(); j++) {
      temp_max=-1;
      for (int k = 1; k < (int) temp_probs.size(); k++) {
        if(temp_probs.at(k)>temp_max){
          temp_max=temp_probs.at(k);
          temp_index=k;
        }
      }
      temp_probs.at(temp_index)=-1;
      temp_indexes.push_back((double)temp_index);
    }
    indexes.push_back(temp_indexes);
  }
  return indexes;
}

int Utils::random_index(int min, int max) {
  /*
   std::srand ( unsigned ( std::time(0) ) );
  std::vector<int> myvector;
  for (int i=1; i<(n+1); i++) myvector.push_back(i);
  std::random_shuffle ( myvector.begin(), myvector.end() );
  return myvector.at(0);
  */
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(min, max);
  return distr(eng);
}

std::vector<std::vector<int>> Utils::read_commands (const char* filename){
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
  sleep(1);
  return comandi;
}

void Utils::stampa_comando(std::vector<int> v){
  InputGenerator ig;
  for (size_t i = 0; i < v.size(); i++) {
    std::cout << ig.parse_label(v.at(i), i);
  }
  std::cout << '\n';
}
std::vector<std::vector<double>> Utils::generic_label_probs(){
  std::vector<std::vector<double>> svm_predictions;
  std::vector<double> temp_v;
  for (size_t i = 0; i < 11; i++) {
    temp_v.push_back(1);
  }
  for (size_t i = 0; i < 10; i++) {
    svm_predictions.push_back(temp_v);
  }
  return svm_predictions;
}

std::vector<std::vector<double>> Utils::remove_label(std::vector<std::vector<double>> probs){
  std::vector<std::vector<double>> svm_predictions;
  std::vector<double> temp_v;
  for (size_t i = 0; i < probs.size(); i++) {
    temp_v.clear();
    for (size_t j = 1; j < 12; j++) {
      temp_v.push_back(probs.at(i).at(j));
    }
    svm_predictions.push_back(temp_v);
  }
  return svm_predictions;
}

std::vector<int> Utils::permutateIndexes(int label) {
  std::srand ( unsigned ( std::time(0) ) );
  std::vector<int> myvector;
  int n_immagini[]={118, 100, 100, 108, 117, 110, 116, 112, 103, 110, 105};
  for (int i=1; i<(int)n_immagini[label]; i++) myvector.push_back(i);
  std::random_shuffle ( myvector.begin(), myvector.end() );
  //std::cout << "Indici mischiati: " << '\n';
  //for (int i=1; i<n_immagini[label]; i++) std::cout << myvector.at(i-1) << " ";
  //std::cout << '\n';
  return myvector;
}

std::vector<std::vector<int>> Utils::generateIndexesVector(){
  std::vector<std::vector<int>> indexes;
  for (int i = 0; i < 11; i++) {
    indexes.push_back(permutateIndexes(i));
  }
  return indexes;
}
