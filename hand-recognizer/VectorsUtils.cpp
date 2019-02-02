#include "VectorsUtils.hpp"

//stampa vettori di vettori di double
void VectorsUtils::print_vec_vec (std::vector<std::vector<double>> vec_vec, bool labels){
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
std::vector<std::vector<double>> VectorsUtils::vec_vec_multip (std::vector<std::vector<double>> vec_a, std::vector<std::vector<double>> vec_b){
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

std::vector<double> VectorsUtils::vec_multip (std::vector<double> vec_a, std::vector<double> vec_b){
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
std::vector<std::vector<double>> VectorsUtils::order_indexes (std::vector<std::vector<double>> labels_probabilities){
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

int VectorsUtils::random_index(int n) {
  /*
   std::srand ( unsigned ( std::time(0) ) );
  std::vector<int> myvector;
  for (int i=1; i<(n+1); i++) myvector.push_back(i);
  std::random_shuffle ( myvector.begin(), myvector.end() );
  return myvector.at(0);
  */
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(91, n);
  return distr(eng);
}
