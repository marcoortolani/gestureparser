//
//  KinectDevice.hpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 21/11/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#ifndef KinectDevice_hpp
#define KinectDevice_hpp

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <random>
#include <string.h>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "json.hpp"
#include "svm.h"


#define WINDOWS_CV_DEPTH_TITLE "Stream Depht Image"
#define WINDOWS_CV_RGB_TITLE "Stream RGB Image"

#define ERROR_ON_OPEN_KINECT "Non posso aprire lo streaming da Kinect.\nControllare che la Kinect sia collegata."
#define ERROR_ON_ACQUISITION_KINECT "Non riesco ad aquisire il frame da Kinect."

#define DATA_JSON_PATH "./data.json"
#define DATA_JSON_JPG "./data.jpg"

//immagini del dataset

#define IMMAGINE_0 "../dataset/dx/0/0_9.tif"
#define IMMAGINE_1 "../dataset/dx/1/1_5.tif"
#define IMMAGINE_2 "../dataset/dx/2/2_9.tif"
#define IMMAGINE_3 "../dataset/dx/3/3_5.tif"

// immagini del testset
/*
#define IMMAGINE_0 "../testset/0/0_9.tif"
#define IMMAGINE_1 "../testset/1/1_5.tif"
#define IMMAGINE_2 "../testset/2/2_9.tif"
#define IMMAGINE_3 "../testset/3/3_5.tif"
*/
#define PI 3.14159265

using json = nlohmann::json;

class KinectDevice{
public:
    //Costruttore
    KinectDevice();

    //funzione acquisizione video
    void acquireRGB();
    void acquireDEPTH();

    //funzione acquisizione frame
    cv::Mat shot();
    //anzichè acquisire il frame con la kinekt passo un file jpg
    void jpgToFrame (int index, int mode, int n_dita, int test_use);
    //funzione elaborazione frame
    cv::Mat shot2bw();
    cv::Mat edge();
    void extractCountours();
    cv::Mat im2bw();
    void drawContourns();
    void frameshow(const char* title = "Frame");

    //funzione estrazione caratteristiche
    void setHandFeatures();
    void p2pDistance();
    cv::Point initialValue();
    std::vector<double> findAngle();
    void normalize();
    void samples();

    //utility
    nlohmann::json genJSON(){
        datas["angolo"] = angolo;
        datas["distanza"] = distanza;
        datas["samples"] = features;
        std::ofstream jf;
        jf.open(DATA_JSON_PATH);
        jf<<datas;
        jf.close();
        cv::imwrite(DATA_JSON_JPG, frame);
        return datas;
    }

    void pressESC(){
        cv::waitKey(0);
    }


    //getter
    int getFrameCols();
    int getFrameRows();
    cv::Mat getFrame();
    std::vector<double> getDistanza();
    const float getScaleFactor();
    cv::VideoCapture getSensor();
    std::vector<std::vector<cv::Point>> getContours();
    std::vector<cv::Vec4i> getHierarchy();
    std::vector<cv::Moments> getMoments();
    std::vector<cv::Point2f> getMassCenter();

    //setter
    void setFrame(cv::Mat f);
    cv::Size setFrameSize();


    //printer
    void printSizeInfo(){
        std::cout<<"Size Contourns: "<<contours.size()<<std::endl;
        std::cout<<"Size Hierarchy: "<<hierarchy.size()<<std::endl;
        std::cout<<"Size Contorno: "<<contorno.size()<<std::endl;
        std::cout<<"Size Moments "<<mu.size()<<std::endl;
        std::cout<<"Size Mass Centers "<<mc.size()<<std::endl;
        std::cout<<"Size Distanza: "<<distanza.size()<<std::endl;
        std::cout<<"Size Angolo: "<<angolo.size()<<std::endl;
        std::cout<<"Size Feature: "<<features.size()<<std::endl;
        std::cout<<"Centroide: "<<centroide<<std::endl;
        std::cout<<"Initial Point: "<<iValue<<std::endl;
    }

private:
    cv::VideoCapture sensor;
    int frame_rows;
    int frame_cols;
    const float scaleFactor = 0.06274509803922f;
    cv::Mat frame;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Moments> mu;
    std::vector<cv::Point2f> mc;
    cv::Point centroide;
    std::vector<cv::Point> contorno;
    cv::Point iValue;
    std::vector<double> distanza;
    std::vector<double> angolo;
    std::vector<double> features;
    nlohmann::json datas;
    std::vector<int>sampling_indexes;

    //private member functions
    double atan2d360(cv::Point P){
        double radians = std::atan2(P.x, P.y);
        double degrees = radians * 180 / M_PI;
        if(radians < 0){
            degrees += 360;
        }
        return degrees;
    }

    double GetUniform(){
        static std::default_random_engine re;
        static std::uniform_real_distribution<double> Dist(0,1);
        //std::cout << "Distribuzione: " << Dist(re) << '\n';
        return Dist(re);
    }

    void SampleWithoutReplacement(int populationSize, int sampleSize,std::vector<int> & samples){
        // Use Knuth's variable names
        int& n = sampleSize;
        int& N = populationSize;

        int t = 0; // total input records dealt with
        int m = 0; // number of items selected so far
        double u;

        while (m < n){
            u = GetUniform(); // call a uniform(0,1) random number generator

            if ( (N - t)*u >= n - m ){
                t++;
            }else{
                samples[m] = t;
                t++; m++;
            }
            //std::cout << "Sample# " << m <<'\n';
        }
    }
};


#endif /* KinectDevice_hpp */
