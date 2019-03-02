#include <iostream>
#include <fstream>

#include "EarleyParser.h"

using std::cout;
using std::endl;

void importGrammarFromFile(const std::string &filename, std::vector<GrammarRule<std::string> > &rules, std::string &start_symbol, std::string &wild_card);

std::vector<std::vector<std::string> > importSentencesFromFile(const std::string &filename);

std::vector<std::string> parse_sentence(std::vector<std::vector<double>> svm_predictions, bool only_parser){
    std::vector<std::string> parsed_sentence;
    std::string grammar_filename("../input/grammar.txt");
    std::string start_symbol, wild_card;
    std::vector<GrammarRule<std::string> > rules;
    importGrammarFromFile(grammar_filename, rules, start_symbol, wild_card);
    std::cout << std::endl;
    EarleyParser<std::string> parser(rules, start_symbol, wild_card);
    std::cout << parser << std::endl;
    std::cout << std::endl;
    std::string sentences_filename("../input/sentence-hr.txt");
    std::vector<std::vector<std::string> > sentences = importSentencesFromFile(sentences_filename);
    std::cout << std::endl ;
    //for(unsigned int kk=0; kk < sentences.size(); kk++){
    for(unsigned int kk=0; kk < 1; kk++){ //una sola frase alla volta
        vector<std::string> original_sentence=sentences[kk];
        bool flag=true;
        bool riconosciuta_parser=true;
        while(flag){
            std::vector<EarleyParser<std::string>::EarleySet> earley_chart;
            std::vector<std::vector<unsigned int> > states_counts = parser.parse(sentences[kk], earley_chart, svm_predictions, only_parser);

            if(states_counts.back().back()!=0){
                flag=false;
                std::cout << std::endl << "Finished parsing." << std::endl << std::endl;

                ParseTree<std::string> viterbi_parse_tree;
                for(unsigned int i = 0; i < earley_chart.back().size(); i++){
                    if(earley_chart.back()[i].isFinished() && (earley_chart.back()[i].rule().LHS() == start_symbol)){
                        std::cout << "-----------------------------------------------------" << std::endl;
                        std::cout << "PARSE-TREE RAPRESENTATION" << std::endl;
                        std::cout << "-----------------------------------------------------\n" << std::endl;
                        std::cout << "Computing parse tree from " << i << "th state in the final set:   " << earley_chart.back()[i] << std::endl << std::endl;
                        viterbi_parse_tree = parser.getViterbiParse(earley_chart.back()[i], earley_chart);
                    }
                }
                viterbi_parse_tree.print(0, 0);

                for(unsigned int i = 0; i < states_counts.size(); i++){
                    for(unsigned int j = 0; j < states_counts[i].size(); j++){
                        std::cout << states_counts[i][j] << " ";
                    }
                    std::cout << std::endl;
                }

                //codice scritto da me
                std::ofstream probabilistic_vector;
                probabilistic_vector.open("probabilistic.bson",std::ofstream::app);
                probabilistic_vector <<"{ ";
                //fine codice scritto da me

                std::cout << "\n\n-----------------------------------------------------" << std::endl;
                std::cout << "TRANSITION PROBABILITIES" << std::endl;
                std::cout << "-----------------------------------------------------\n" << std::endl;
                for(unsigned int i = 0; i < earley_chart.size(); i++){
                    std::vector<std::pair<std::string, double> > transition_probs = parser.getNextWordTransitions(earley_chart[i]);
                    probabilistic_vector <<"\""<<i<<"\": { ";
                    for(unsigned int j = 0; j < transition_probs.size(); j++){
                        //queste sono le probabilità che mi interessa prendere
                        //codice scrtto da me
                        if(transition_probs[j].first == ";" || j == transition_probs.size()-1){
                            probabilistic_vector <<"\""<<transition_probs[j].first << "\":" << transition_probs[j].second;
                        }else{
                            probabilistic_vector <<"\""<<transition_probs[j].first << "\":" << transition_probs[j].second << ",    ";
                        }
                        //fine codice scirtto da me
                        std::cout << transition_probs[j].first << " --> " << transition_probs[j].second << "    "<<std::endl;
                    }
                    //codice scritto da me
                    if(i == earley_chart.size()-1){
                        probabilistic_vector << "}";
                    }else{
                        probabilistic_vector << "},";
                    }
                    //fine codice scritto da me
                    std::cout << std::endl;
                }
                //codice scritto da me
                probabilistic_vector << "}" <<std::endl;
                //fine codice scritto da me

                //std::ofstream f;
                //f.open("command.txt",std::ofstream::app);
                //if(!f){
                //     std::cout << "Errore nella creazione del file!";
                //     parsed_sentence.clear();
                //     return parsed_sentence;
                // }else{
                    // for(unsigned int i = 0; i < original_sentence.size(); i++)
                    // {
                    //     f << original_sentence[i] << " ";
                    // }

                    //f << "--> ";
                    parsed_sentence.clear();
                    for(unsigned int j = 0; j < sentences[kk].size(); j++){
                        //f << sentences[kk][j] << " ";
                        parsed_sentence.push_back(sentences[kk][j]);
                    }

                    //f << std::endl;
                //}
                //f.close();
            }else{
                //stringa errata
                std::vector<unsigned int> vt;
                std::cout << std::endl;
                for(unsigned int i = 0; i < states_counts.size(); i++){
                    unsigned int ccc = 0;
                    for(unsigned int j = 0; j < states_counts[i].size(); j++){
                        ccc+=states_counts[i][j];
                    }
                    vt.push_back(ccc);
                }

                unsigned int index;
                for(unsigned int k = 0; k < vt.size(); k++){
                    if(vt[k]==0){
                        index=k;
                        break;
                    }
                }

                std::vector<std::pair<std::string, double> > transition_probs = parser.getNextWordTransitions(earley_chart[index-1]);
                if (transition_probs.size()!=0){
                std::string symbol_corr=transition_probs[0].first;
                  double max_prob=transition_probs[0].second;
                  for(unsigned int j = 0; j < transition_probs.size(); j++){
                      if(transition_probs[j].second > max_prob){
                          symbol_corr=transition_probs[j].first;
                          max_prob=transition_probs[j].second;
                      }
                  }
                sentences[kk].at(index-1)=symbol_corr;
                std::cout << symbol_corr << " --> " << max_prob << std::endl;
              }else{
                  std::cout << "Frase non riconosciuta" << '\n';
                  flag=false;
                  riconosciuta_parser=false;
                  parsed_sentence.clear();
              }
            }
        }
        std::cout << "\n\nParsed sentence\n";
        if(riconosciuta_parser){
          for(unsigned int j = 0; j < sentences[kk].size(); j++) {
                  std::cout << sentences[kk][j] << " ";
          }
          std::cout << "\n\n";
        } else {
          std::cout << "Il parser non ha riconosciuto il comando\n\n";
        }
    }
    return parsed_sentence;
}


void importGrammarFromFile(const std::string &filename, std::vector<GrammarRule<std::string> > &rules, std::string &start_symbol, std::string &wild_card){
    unsigned int line_count = 1;
    std::string LHS;
    std::vector<std::string> RHS;
    double prob;
    std::string temp;
    std::string symbol;
    std::ifstream in(filename.c_str(), std::ios::in);
    wild_card = "$";

    if (!in.is_open()){
        std::cout << "Error opening file " << filename << std::endl;
        exit(1);
    }

    while(!in.eof()){
        std::string line;
        getline(in, line);

        if(line_count==1){
            std::stringstream ss(line);
            ss >> start_symbol;
            ss >> wild_card;
            line_count++;
        }else{
            std::stringstream ss(line);

            if(line.size() == 0)
                continue;

            ss >> LHS;

            ss >> temp;
            if(temp != "-->"){
                std::cout << "line " << line_count << ": [" << line << "] is not a rule." << std::endl;
                exit(1);
            }

            while(!ss.eof()){
                ss >> symbol;
                RHS.push_back(symbol);
            }

            prob=atof(RHS.back().substr(1,symbol.size()-1).c_str());
            RHS.pop_back();

            rules.push_back(GrammarRule<std::string>(LHS, RHS, prob));
            RHS.clear();
            line_count++;
        }

    }
    std::cout << rules.size() << " rules loaded:" << std::endl;

}


std::vector<std::vector<std::string> > importSentencesFromFile(const std::string &filename){
    std::ifstream in(filename.c_str(), std::ios::in);
    if (!in.is_open()){
        std::cout << "Error opening file " << filename << std::endl;
        exit(1);
    }

    std::vector<std::vector<std::string> > sentences;
    unsigned int line_count = 0;
    while(!in.eof()){
        line_count++;

        std::string line;
        getline(in, line);
        std::stringstream ss(line);

        if(line.size() == 0)
            continue;

        std::vector<std::string> sentence;
        while(!ss.eof())
        {
            std::string symbol;
            ss >> symbol;
            sentence.push_back(symbol);
        }

        sentences.push_back(sentence);
    }

    std::cout << sentences.size() << " sentence loaded:" << std::endl;

    return sentences;
}
