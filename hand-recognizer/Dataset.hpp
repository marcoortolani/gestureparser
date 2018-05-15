//
//  Dataset.hpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 05/12/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#ifndef Dataset_hpp
#define Dataset_hpp

#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include "json.hpp"
#include <chrono>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


#define MODE_CREATE_DATASET 0


#define DATASET_DIR "\"../dataset\""
#define DATASET_SVM "../dataset/dataset"
#define DATASET_SVM_MODEL "../dataset/dataset.model"
#define DATASET_SVM_MODEL_NEW "../dataset/dataset_new.model"
#define SCALING_MODEL 0
#define DATASET_SVM_MODEL_SCALED "../dataset/dataset.scale.model"
#define DATASET_PATH "../dataset/dataset.json"
#define TESTSET_SVM "../dataset/testset"
#define OUTPUT_PROBABILISTIC_SVM "../dataset/prob.khr"



#define DX_HAND 1
#if DX_HAND
#define dir_dx_0  "../dataset/dx/0"
#define dir_dx_1  "../dataset/dx/1"
#define dir_dx_2  "../dataset/dx/2"
#define dir_dx_3  "../dataset/dx/3"
#define dir_dx_4  "../dataset/dx/4"
#define dir_dx_5  "../dataset/dx/5"
#endif

/*#define SX_HAND 0
#if SX_HAND
#define dir_sx_0  "../dataset/sx/0"
#define dir_sx_1  "../dataset/sx/1"
#define dir_sx_2  "../dataset/sx/2"
#define dir_sx_3  "../dataset/sx/3"
#define dir_sx_4  "../dataset/sx/4"
#define dir_sx_5  "../dataset/sx/5"
#endif*/



using namespace std;
using json = nlohmann::json;
using namespace std::chrono;


class Dataset{
public:
    Dataset();
    Dataset(const char* file_name);

    //setter
    void setFileName(const char* file_name);
    void setDataObj(std::vector<nlohmann::json> jobjs);
    void setNumberOfRows();

    //getter
    const char* getFileName();
    std::vector<nlohmann::json> getDataObjs();
    double getNumberOfRows();

    //methods
    void updateNumberOfRows();
    void writeOn(nlohmann::json sample);
    void writeOn(nlohmann::json sample,const char* dir);
    void writeOn(nlohmann::json sample,const char* dir,cv::Mat img);
    void parseObjsFile();
    void saveElement(nlohmann::json sample, cv::Mat img);
    void genSVMDataset();
    void saveGesture(int n_dita, nlohmann::json jline,const char* filename = TESTSET_SVM);

private:
    const char* fname;
    double nrows_dataset;
    ifstream rdfile;
    ofstream dfile;
    std::vector<nlohmann::json> dataObjs;

};

#endif /* Dataset_hpp */
