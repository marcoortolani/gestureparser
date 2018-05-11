//
//  Dataset.cpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 05/12/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#include "Dataset.hpp"

//costruttori
Dataset::Dataset(){
    fname = DATASET_PATH;
    updateNumberOfRows();
}


Dataset::Dataset(const char* file_name){
    fname = file_name;
    updateNumberOfRows();
}


//getter
const char* Dataset::getFileName(){
    return fname;
}

std::vector<nlohmann::json> Dataset::getDataObjs(){
    return dataObjs;
}

double Dataset::getNumberOfRows(){
    return nrows_dataset;
}

//setter

void Dataset::setFileName(const char* file_name){
    fname = file_name;
}

void Dataset::setDataObj(std::vector<nlohmann::json> jobjs){
    dataObjs = jobjs;
}

void Dataset::setNumberOfRows(){
    updateNumberOfRows();
}

//methods

void Dataset::updateNumberOfRows(){
    rdfile.open(fname);
    nrows_dataset = std::count(std::istreambuf_iterator<char>(rdfile),std::istreambuf_iterator<char>(), '\n');
    rdfile.close();
}


void Dataset::writeOn(nlohmann::json sample){
    dfile.open(DATASET_PATH,std::ofstream::out | std::ofstream::app);
    dfile<<sample<<std::endl;
    dfile.close();
    updateNumberOfRows();
}

void Dataset::writeOn(nlohmann::json sample,const char* dir){
    writeOn(sample);
    std::stringstream json_file;
    json_file<<dir<<"/subset.json";
    dfile.open(json_file.str().c_str(),std::ofstream::out | std::ofstream::app);
    dfile<<sample<<std::endl;
    dfile.close();
}

void Dataset::writeOn(nlohmann::json sample,const char* dir, cv::Mat img){
    std::stringstream jpg_file;
    milliseconds ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    jpg_file <<dir<<"/"<<ms.count()<<".jpg";
    writeOn(sample,dir);
    cv::imwrite(jpg_file.str().c_str(), img);
}


void Dataset::parseObjsFile(){
    updateNumberOfRows();
    std::vector<nlohmann::json> tmpObjs(nrows_dataset);
    if (nrows_dataset == 0) {
        std::cout<<"No Data."<<std::endl;
        exit(EXIT_FAILURE);
    }
    rdfile.open(fname);
    for (int i = 0 ; i < nrows_dataset; i++) {
        std::string temp;
        getline(rdfile, temp, '\n');
        tmpObjs[i] = temp;
    }
    dataObjs = tmpObjs;
}

void Dataset::saveElement(nlohmann::json sample, cv::Mat img){
    char position;
    int label;

    //std::cout<<"d | s: ";
    //std::cin>>position;
    //std::cout<<std::endl;
    position = 'd';
    switch (position) {
        case 'd':
            std::cout<<"Dita [0-5]: ";
            std::cin>>label;
            std::cout<<std::endl;
            switch(label){
                case 0:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_0,img);
                    break;
                case 1:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_1,img);
                    break;
                case 2:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_2,img);
                    break;
                case 3:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_3,img);
                    break;
                case 4:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_4,img);
                    break;
                case 5:
                    sample["label"] = label;
                    sample["position"] = "dx";
                    writeOn(sample,dir_dx_5,img);
                    break;
                case 9:
                    exit(EXIT_FAILURE);
                    break;
                default:
                    exit(EXIT_FAILURE);
                    break;
            }
            break;
        case 'n':
            exit(EXIT_FAILURE);
            break;
        default:
            break;
    }
}

void Dataset::saveGesture(nlohmann::json jline,const char* filename){
    std::ofstream test;
    std::ofstream testjson;
    test.open(filename,std::ofstream::out | std::ofstream::app);
    std::string ftestjson = TESTSET_SVM;
    ftestjson.append(".json");
    testjson.open(ftestjson.c_str(),std::ofstream::out | std::ofstream::app);
    testjson<<jline;
    testjson.close();
    test <<5<<" ";
    for (int i = 0; i < jline["samples"].size(); i++) {
        test<<i+1<<":"<<jline["samples"].at(i)<<" ";
    }
    test << std::endl;
    test.close();
}


void Dataset::genSVMDataset(){
    rdfile.open(DATASET_PATH);
    dfile.open(DATASET_SVM,std::ofstream::out | std::ofstream::app);
    std::string sline;
    nlohmann::json jline;
    while(std::getline(rdfile,sline)){
        jline = json::parse(sline);
        dfile << jline["label"]<<" ";
        for (int i = 0; i < jline["samples"].size(); i++) {
            dfile<<i+1<<":"<<jline["samples"].at(i)<<" ";
        }
        dfile << std::endl;
    }
    dfile.close();
    rdfile.close();
    std::cout<<"Before svm_train, remember to order dataset; paste in terminal:\n\t> cd ../dataset\n\t> sort_dataset"<<std::endl;
}
