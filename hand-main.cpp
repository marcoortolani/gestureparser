//
//  main.cpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 21/11/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#include "hand-recognizer.h"
#include "train.h"
#include <unistd.h>

using namespace std;
using namespace cv;


void print_menu(){
    std::cout<<"Kinect Hand Recognizer\n\n"
            <<"1) Start Simulation\n"
            <<"2) Parse Output SVM\n"
            <<"3) Create Dataset\n"
            <<"4) Test testset\n"
            <<"5) SVM Train\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}

void simulation(KinectDevice& kinect, Dataset& dataset, int img_number, int mode, int n_dita, int test_use){
    kinect.jpgToFrame(img_number, mode, n_dita, test_use);
    //kinect.shot2bw();
    //estraggo contours e hierarchy

    // Edge già estratto nel dataset di prova
    //kinect.edge();
    kinect.extractCountours();

    kinect.setHandFeatures();
    kinect.initialValue();
    kinect.findAngle();
    kinect.normalize();
    kinect.samples();
    kinect.printSizeInfo();
    nlohmann::json test = kinect.genJSON();
    dataset.saveGesture(n_dita, test);
}

void training(KinectDevice& kinect, Dataset& dataset, int img_number, int mode, int n_dita){
    kinect.jpgToFrame(img_number, mode, n_dita, 0);
    //kinect.shot2bw();
    //estraggo contours e hierarchy

    // Edge già estratto nel dataset di prova
    //kinect.edge();
    kinect.extractCountours();

    kinect.setHandFeatures();
    kinect.initialValue();
    kinect.findAngle();
    kinect.normalize();
    kinect.samples();
    //kinect.printSizeInfo();
    nlohmann::json test = kinect.genJSON();
    dataset.saveGesture(n_dita, test);
}

void dataset_creator(KinectDevice& kinect, Dataset& dataset){
    kinect.shot2bw();
    //estraggo contours e hierarchy
    //kinect.edge();
    kinect.extractCountours();

    kinect.setHandFeatures();
    kinect.initialValue();
    kinect.findAngle();
    kinect.normalize();
    kinect.samples();
    kinect.printSizeInfo();
    nlohmann::json test = kinect.genJSON();
    dataset.saveElement(test, kinect.getFrame());
}

void exit_with_prompt(){
    std::exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
    // Apro la connessione con la kinect.
    Dataset dataset;
    InputGenerator command_exec;
    KinectDevice kinect;
    int opt;
    char exec_quit = 'n';
    int scegli_dataset, img_number;
    do {
        print_menu();
        opt=0;
        std::cin>>opt;
        switch (opt) {
            {case 1:
                //cancello i files che mantengono i dati delle precedenti simulazioni
                std::remove("../dataset/testset.json");
                std::remove("../dataset/testset");
                std::remove("../dataset/prob.khr");
                std::remove("../input/sentence-hr.txt");
                img_number=0;
                while(img_number<4){
                    //system(COMMAND_WEB_VISUALIZER);
                    simulation(kinect, dataset, img_number, 0, img_number, 0);
                    img_number++;
                    std::cout<<"WAIT\n";
                    sleep(2);
                }
                gesture_prediction(TESTSET_SVM,DATASET_SVM_MODEL,OUTPUT_PROBABILISTIC_SVM);
                break;
            }
            case 2:
                command_exec.parse(OUTPUT_PROBABILISTIC_SVM);
                break;

            case 3:
                std::cout<<"disabilitato\n";
                //dataset_creator(kinect, dataset);
                break;
            case 4:
              std::remove("../dataset/testset.json");
              std::remove("../dataset/testset");
              std::remove("../dataset/prob.khr");
              std::remove("../input/sentence-hr.txt");
              img_number=1;
              int use_dataset;
              std::cout << "Quale modello uso? (0-vecchio 1-nuovo)\n";
              std::cin >>use_dataset;
              std::cout << "Quante dita? (0-5)" << std::endl;
              std::cin >> scegli_dataset;
              while(img_number<31){
                  //system(COMMAND_WEB_VISUALIZER);
                  simulation(kinect, dataset, img_number, 1, scegli_dataset, 1);
                  img_number++;
              }
              if (use_dataset==0){
                std::cout<<"use_dataset"<<use_dataset<<std::endl;
                gesture_prediction(TESTSET_SVM,DATASET_SVM_MODEL,OUTPUT_PROBABILISTIC_SVM);
              }
              else{
              std::cout<<"use_dataset"<<use_dataset<<std::endl;
              gesture_prediction(TESTSET_SVM,DATASET_SVM_MODEL_NEW,OUTPUT_PROBABILISTIC_SVM);
            }
                break;
            case 5:
              std::remove("../dataset/testset.json");
              std::remove("../dataset/testset");
              std::remove("../dataset/prob.khr");
              std::remove("../input/sentence-hr.txt");
              scegli_dataset=0;
              for (scegli_dataset=0; scegli_dataset<6; scegli_dataset++){
                for(img_number=1; img_number<31; img_number++){
                    training(kinect, dataset, img_number, 1, scegli_dataset);
                    //sleep(1);
                }
              }
              train(TESTSET_SVM,DATASET_SVM_MODEL_NEW);
              std::cout << "Vuoi testare il modello appena creato? (y/n)\n(Verranno usate le stesse features usate nell'addestramento)" << '\n';
              char testare;
              std::cin >> testare;
              if (testare=='y' || testare=='Y'){
                gesture_prediction(TESTSET_SVM,DATASET_SVM_MODEL_NEW,OUTPUT_PROBABILISTIC_SVM);
              }
                break;
            case 6:
              train("../dataset/dataset", DATASET_SVM_MODEL_NEW);
              gesture_prediction("../dataset/dataset",DATASET_SVM_MODEL_NEW,OUTPUT_PROBABILISTIC_SVM);
            break;
            case 9:
                exit(1);
            default:
                std::cout<<"Invalid command.\n";
                break;
        }
        std::cout<<"Quit [n(default)/y]: ";
        std::cin>>exec_quit;
    } while (exec_quit != 'y' );
    return 0;
}
