//
//  KinectDevice.cpp
//  Kinect Hand Recognizer
//
//  Created by Francesco Lanza on 21/11/16.
//  Copyright © 2016 Francesco Lanza. All rights reserved.
//

#include "KinectDevice.hpp"

KinectDevice::KinectDevice(){
    // sensor.open(CV_CAP_OPENNI);
    // if( !sensor.isOpened() ){
    //     std::cout << ERROR_ON_OPEN_KINECT << std::endl;
    //     exit(-1);
    // }
}

void KinectDevice::setFrame(cv::Mat f){
    frame = f.clone();
}

int KinectDevice::getFrameCols(){
    return frame_cols;
}

int KinectDevice::getFrameRows(){
    return frame_rows;
}

cv::Mat KinectDevice::getFrame(){
    return frame;
}

const float KinectDevice::getScaleFactor(){
    return scaleFactor;
}

cv::VideoCapture KinectDevice::getSensor(){
    return sensor;
}

std::vector<std::vector<cv::Point>> KinectDevice::getContours(){
    return contours;
}


std::vector<cv::Vec4i> KinectDevice::getHierarchy(){
    return hierarchy;
}

std::vector<double> KinectDevice::getDistanza(){
    return distanza;
}

void KinectDevice::frameshow(const char* title){
    cv::namedWindow(title,CV_WINDOW_AUTOSIZE);
    cv::imshow(title, frame);
}

void KinectDevice::acquireDEPTH(){
    while(cv::waitKey(30) != 27){
        cv::imshow(WINDOWS_CV_DEPTH_TITLE,shot());
    }
}

void KinectDevice::acquireRGB(){
    while(cv::waitKey( 30 )!= 27){
        cv::Mat img;
        if( !sensor.grab() ){
            std::cout << ERROR_ON_ACQUISITION_KINECT << std::endl;
        }else if(sensor.retrieve( img, CV_CAP_OPENNI_BGR_IMAGE )){
            imshow(WINDOWS_CV_RGB_TITLE,img);
        }
    }
}

cv::Mat KinectDevice::shot(){
    if( !sensor.isOpened() ){
        sensor.open(CV_CAP_OPENNI);
        if( !sensor.isOpened() ){
            std::cout << ERROR_ON_OPEN_KINECT << std::endl;
            exit(-1);
        }
    }
    sensor >> frame;
    frame.convertTo( frame, CV_8UC1, scaleFactor );
    sensor.release();
    return frame;
}

void KinectDevice::jpgToFrame (int index, int mode, int n_dita, int test_use){
cv::Mat image;
if (mode==0){           //simulation
  const char* path[4];
  path[0]=IMMAGINE_0;
  path[1]=IMMAGINE_1;
  path[2]=IMMAGINE_2;
  path[3]=IMMAGINE_3;
  std::cout << "path: " << path[index] << '\n';
  image = cv::imread(path[index], 0 );

} else{               //test testset oppure create model
    char integer_string[20];
  	sprintf(integer_string, "%d", n_dita);
    char percorso[50] ="";
    char percorso_est[50] ="";
    //if (test_use==1)
  	// strcat(percorso, "../testset/");
    //else
    strcat(percorso, "../dataset/dx/");
    strcat(percorso, integer_string);
    //strcat(percorso, "/OK/"); //uso le 10 immagini scelte dal testset
    strcat(percorso, "/");  //uso tutte le immagini del testset
    strcat(percorso, integer_string);
    strcat(percorso, "_");
    sprintf(integer_string, "%d", index);
    strcat(percorso, integer_string);
    strcat(percorso_est, percorso);
    strcat(percorso_est, ".tif");
    std::cout << "Reading in path: " << percorso_est << '\n';
    image = cv::imread(percorso_est, 0);
    // //per salvare le immagini in jpg senza compressione
    // strcat(percorso, ".jpg");
    // std::cout << "Whrite in path: " << percorso << '\n';
    // std::vector<int> parametri;
    // parametri.push_back(100);
    // cv::imwrite(percorso, image, parametri);
}
//image.convertTo( frame, CV_8UC1, scaleFactor );
frame=image;
//im2bw();
}

cv::Size KinectDevice::setFrameSize(){
    cv::Mat tmp;
    sensor >> tmp;
    cv::Size dimensions = tmp.size();
    frame_cols = tmp.cols;
    frame_rows = tmp.rows;
    return dimensions;
}

cv::Mat KinectDevice::edge(){
    cv::Mat edged = frame.clone();
    cv::GaussianBlur(edged, edged, cv::Size(7,7), 1.5, 1.5);
    cv::Canny(edged, edged, 0, 120, 3);
    //cv::imshow("Edged", edged);
    //cv::waitKey(20000);
    return edged;
}

cv::Mat KinectDevice::im2bw(){
    double minVal=-1;
    double maxVal=-1;
    cv::Mat dst;
    cv::Mat temp = frame.clone();
    temp.setTo(255, temp == 0);
    cv::minMaxIdx(temp, &minVal, &maxVal);
    int delta = 10 - ((int)round(minVal) % 10);
    cv::inRange(frame, cv::Scalar(minVal), cv::Scalar(minVal+delta), dst);
    frame = dst.clone();
    return dst;
}

void KinectDevice::extractCountours(){
    cv::findContours(frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));
    //cv::imshow("Extract Contours", frame);
    //cv::waitKey(20000);
}

std::vector<cv::Moments> KinectDevice::getMoments(){
    std::vector<cv::Moments> temp(contours.size());
    for( int i = 0; i < contours.size(); i++ ){
        temp[i] = moments( contours[i], false );
    }
    mu = temp;
    return temp;
}

std::vector<cv::Point2f> KinectDevice::getMassCenter(){
    if(mu.size() == 0){
        getMoments();
    }
    std::vector<cv::Point2f> temp( contours.size() );
    for( int i = 0; i < contours.size(); i++ ){
        temp[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
    }
    mc = temp;
    return temp;
}

void KinectDevice::drawContourns(){
    // Draw contours
    cv::Scalar color = cv::Scalar(240,128,30);
    cv::Mat drawing = cv::Mat::zeros( frame.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ ){
        cv::drawContours( drawing, contours, i, color, 1, 1, hierarchy, 0, cv::Point());
    }

    // Show in a window
    cv::namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    cv::imshow( "Contours", drawing );
    cv::waitKey(2000);
}

void KinectDevice::setHandFeatures(){
    getMoments();
    getMassCenter();
    contorno = contours.at(0);
    centroide.x = round(mc.at(0).x);
    centroide.y = round(mc.at(0).y);
    p2pDistance();
}

void KinectDevice::p2pDistance(){
    std::vector<double> temp(contorno.size());
    std::vector<cv::Point> src2(1);
    src2.insert(src2.end(),centroide);
    for(int i = 0; i < contorno.size() ; i++){
        std::vector<cv::Point> src1(1);
        src1.insert(src1.end(),contorno.at(i));
        temp.at(i) = cv::norm(src1,src2,cv::NORM_L2, cv::noArray());
        //std::cout<<"Point "<<i<<": "<<contorno.at(i)<<" - Centroide: "<<centroide<<" "<<"Distanza: "<<temp.at(i)<<std::endl;
        //std::cout<<std::endl;
    }
    distanza = temp;
}


cv::Point KinectDevice::initialValue(){
    //max y, min x;
    iValue = contorno.at(0);
    return iValue;
}

std::vector<double> KinectDevice::findAngle(){
    std::vector<double> temp(contorno.size());
    cv::Point iValueT = iValue - centroide;

    double varAngle = atan2d360(iValueT);
    cv::Mat Mrot = cv::Mat::eye(2, 2, CV_32S);
    Mrot.at<double>(0, 0) =      std::cos(varAngle);
    Mrot.at<double>(0, 1) = (-1)*std::sin(varAngle);
    Mrot.at<double>(1, 0) =      std::sin(varAngle);
    Mrot.at<double>(1, 1) =      std::cos(varAngle);

    cv::Point Princ;
    Princ.x = Mrot.at<double>(0, 0) * iValueT.x + Mrot.at<double>(0, 1) * iValueT.y;
    Princ.y = Mrot.at<double>(1, 0) * iValueT.x + Mrot.at<double>(1, 1) * iValueT.y;

    double principal;
    principal = atan2d360(Princ);

    for (int i = 0 ; i < contorno.size() ; i++) {
        cv::Point Pt = contorno.at(i) - centroide;
        cv::Point V;
        V.x = Mrot.at<double>(0, 0) * Pt.x + Mrot.at<double>(0, 1) * Pt.y;
        V.y = Mrot.at<double>(1, 0) * Pt.x + Mrot.at<double>(1, 1) * Pt.y;
        temp.at(i) = floor(atan2d360(V) - principal);
    }
    angolo = temp;
    return angolo;

}

void KinectDevice::normalize(){
    double minVal = 0;
    cv::Point minIdx;

    minMaxLoc(angolo, &minVal,NULL,&minIdx,NULL,cv::noArray());
    minVal = std::abs(minVal);
    cv::add(angolo,minVal,angolo);
    minMaxLoc(angolo, NULL,NULL,&minIdx,NULL,cv::noArray());

    std::vector<double> _angolo(angolo.begin()+minIdx.x,angolo.end());
    _angolo.insert(_angolo.end(),angolo.begin(), angolo.begin()+minIdx.x);

    std::vector<double> _distanza(distanza.begin()+minIdx.x,distanza.end());
    _distanza.insert(_distanza.end(), distanza.begin(), distanza.begin()+minIdx.x);

    angolo = _angolo;
    distanza = _distanza;
}

void KinectDevice::samples(){
    const size_t sz = 120; // campionamento ogni 3 gradi
    std::vector<int> temp(sz);
    std::vector<double> sampled_signal(sz);
    SampleWithoutReplacement(angolo.size(),sz,temp);
    for (size_t i = 0; i < sz; i++ ) {
        if(i < distanza.size()){
            sampled_signal.at(i) = distanza.at(temp[i]);
        };
    }
    features = sampled_signal;
    sampling_indexes = temp;
}

cv::Mat KinectDevice::shot2bw(){
    shot();
    im2bw();
    return frame;
}
