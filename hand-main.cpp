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

void simulation(KinectDevice& kinect, Dataset& dataset, int index, int mode, int n_dita){
    kinect.jpgToFrame(index, mode, n_dita);
    //kinect.shot2bw();
    //estraggo contours e hierarchy

    // Edge già estratto nel dataset di prova
    kinect.edge();
    kinect.extractCountours();

    kinect.setHandFeatures();
    kinect.initialValue();
    kinect.findAngle();
    kinect.normalize();
    kinect.samples();
    kinect.printSizeInfo();
    nlohmann::json test = kinect.genJSON();
    dataset.saveGesture(n_dita, test);
    gesture_prediction(TESTSET_SVM,DATASET_SVM_MODEL,OUTPUT_PROBABILISTIC_SVM);

}

void training(KinectDevice& kinect, Dataset& dataset, int index, int mode, int n_dita){
    kinect.jpgToFrame(index, mode, n_dita);
    //kinect.shot2bw();
    //estraggo contours e hierarchy

    // Edge già estratto nel dataset di prova
    kinect.edge();
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
    int j, i = 0;
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
                while(i<4){
                    //system(COMMAND_WEB_VISUALIZER);
                    simulation(kinect, dataset, i, 0, i);
                    i++;
                    std::cout<<"WAIT\n";
                    usleep(50000);
                }
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
              i=1;
              std::cout << "Quante dita? (0-5)" << std::endl;
              std::cin >> j;
              while(i<11){
                  //system(COMMAND_WEB_VISUALIZER);
                  simulation(kinect, dataset, i, 1, j);
                  i++;
              }
                break;
            case 5:
              std::remove("../dataset/testset.json");
              std::remove("../dataset/testset");
              std::remove("../dataset/prob.khr");
              std::remove("../input/sentence-hr.txt");

              for (j=0; j<6; j++){
                for(i=1; i<11; i++){
                    training(kinect, dataset, i, 1, j);
                }
              }
              train(TESTSET_SVM,DATASET_SVM_MODEL_NEW);
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
