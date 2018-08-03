//============================================================================
// Name        : Inference_PCFG.cpp
// Author      : MTM
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#define NDEBUG

#include <iostream>
#include <string>
#include <fstream>

#include <boost/unordered_set.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp> // to cast a string to unsigned

#include "Signature.hpp"
#include "ProbabilisticContextFreeGrammar.hpp"
#include "PCFGRule.hpp"
#include "InsideOutsideCalculator.hpp"
#include "InsideOutsideCache.hpp"
#include "EMTrainer.hpp"

int main(int argc, const char * argv[])
{

    // typedef boost::char_separator<char> CharSeparator;
    // typedef boost::tokenizer<CharSeparator> Tokenizer;
    // typedef std::vector<std::string> StringVector;

    if((argc==2) && (strcmp(argv[1],"-h")==0))
    {
    	std::cout << "\nIstruzioni Inference_PCFG:\n";
    	std::cout << "-g [grammar_in] : file della grammatica da elaborare;\n";
    	std::cout << "-t [training_set] : file con le stringhe da utilizzare per l'addestramento;\n";
    	std::cout << "-s [grammar_out] : file su cui scrivere la grammatica modificata;\n";
    	return 0;
    }
    else if ((argc==7) && (strcmp(argv[1],"-g")==0) && (strcmp(argv[3],"-t")==0) && (strcmp(argv[5],"-s")==0))
    {
    	std::ifstream grammar_file;
    	std::string grammar_in = argv[2];

     	grammar_file.open(grammar_in.c_str(), std::ios::in);
    	if (grammar_file) {

    		std::ifstream training_file;
    		std::string training_set = argv[4];
    		training_file.open(training_set.c_str(), std::ios::in);

    		if (training_file) {
    			// Read in grammar
    			ProbabilisticContextFreeGrammar grammar(grammar_file);

    			// Initialize the EMTrainer
    			EMTrainer trainer(grammar, training_file);

    			trainer.train(unsigned(3)); // default

    			std::string grammar_out = argv[6];

    			std::ofstream save_file(grammar_out);

    			if (save_file) {
    				save_file << grammar;
    				save_file.close();
    				return 0;
    			} else {
    				std::cerr << "Impossibile scrivere sul file di output: '" << argv[6] << "'";
    				return 1;
    			}
    		} else {
    			std::cerr << "Impossibile leggere dall'insieme di training: '" << argv[4] << "'";
    			return 1;
    		}
    	} else {
    		std::cerr << "Impossibile leggere la grammatica di input: '" << argv[2] << "'";
    		return 1;
    	}
    }
    else {
    	std::cerr << "\nNUMERO DI PARAMETRI ERRATO...\n";
    	std::cout << "Istruzioni Inference_PCFG:\n";
    	std::cout << "-g [grammar_in] : file della grammatica da elaborare;\n";
    	std::cout << "-t [training_set] : file con le stringhe da utilizzare per l'addestramento;\n";
    	std::cout << "-s [grammar_out] : file su cui scrivere la grammatica modificata;\n";
    	return 1;
    }
}
