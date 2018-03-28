#include <unistd.h>
#include <math.h>

#include "hand-recognizer.h"

#define DATA_JSON_PATH "./data.json"
#define DATA_JSON_JPG "./data.jpg"

using namespace std;
using namespace cv;

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
    }
}

int main(int argc, char* argv[]) {
  const float scaleFactor = 0.06274509803922f;

//  string imgPath = "../dataset/dx/0/1481713432723.jpg";
  string imgPath = "../dataset/dx/1/1481713360075.jpg";

  cout << "Reading in image: " << imgPath << endl;

  // Read image from file
  Mat image = cv::imread(imgPath, -1);

  cv::imshow("Original image", image);
  cv::waitKey(2000);

  // Extract only first channel
  Mat image2;
  image.convertTo( image2, CV_8UC1, scaleFactor );

  // Apply threshold and convert to BW [jpgToFrame()]
  Mat image3;

  double minVal=-1;
  double maxVal=-1;
  image2.setTo(255, image2 == 0);
  cv::minMaxIdx(image2, &minVal, &maxVal);
  cv::imshow("Black and white", image2);
  cv::waitKey(2000);
  int delta = 10 - ((int)round(minVal) % 10);

  cv::inRange(image2, cv::Scalar(minVal), cv::Scalar(minVal+delta), image3);

  cv::imshow("Edged", image3);
  cv::waitKey(2000);

  // Extract image contour [extractCountours()]
  std::vector<std::vector<cv::Point>> contours;
  std::vector<cv::Vec4i> hierarchy;

  cv::findContours(image3, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

  // Compute centroid [setHandFeatures()]
  std::vector<cv::Moments> mu;
  std::vector<cv::Point2f> mc;
  cv::Point centroide;
  std::vector<cv::Point> contorno;

  // getMoments() ---> CORREGGERE
  std::vector<cv::Moments> tempMoments(contours.size());
  for( int i = 0; i < contours.size(); i++ ){
      tempMoments[i] = moments( contours[i], false );
  }
  mu = tempMoments;

  // getMassCenter() ---> CORREGGERE
  std::vector<cv::Point2f> tempPoints( contours.size() );
  for( int i = 0; i < contours.size(); i++ ){
      tempPoints[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
  }
  mc = tempPoints;

  contorno = contours.at(0);
  centroide.x = round(mc.at(0).x);
  centroide.y = round(mc.at(0).y);

  cv::Point iValue = contorno.at(0);

  cout << "Centroid: " << mc << endl;
  cout << "Initial point: " << iValue << endl;

  // p2pDistance() ---> CORREGGERE
  std::vector<double> distanza;
  std::vector<double> angolo;
  std::vector<double> features;

  std::vector<double> tempDistanze(contorno.size());
  std::vector<cv::Point> src2(1);
  src2.insert(src2.end(),centroide);

  for(int i = 0; i < contorno.size() ; i++){
      std::vector<cv::Point> src1(1);
      src1.insert(src1.end(),contorno.at(i));
      tempDistanze.at(i) = cv::norm(src1,src2,cv::NORM_L2, cv::noArray());
      std::cout<<"Point "<<i<<": "<<contorno.at(i)<<" - Centroide: "<<centroide<<" "<<"Distanza: "<<tempDistanze.at(i)<<std::endl;
      std::cout<<std::endl;
  }
  distanza = tempDistanze;

  // findAngle()
  std::vector<double> tempAngoli(contorno.size());
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
      tempAngoli.at(i) = (atan2d360(V) - principal);
  }
  angolo = tempAngoli;

  // normalize()
  minVal = 0;
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

  // samples()
  std::vector<int>sampling_indexes;
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

  // printSizeInfo()
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

  // genJSON() (in KinectDevice.hpp)
  nlohmann::json datas;
  datas["angolo"] = angolo;
  datas["distanza"] = distanza;
  datas["samples"] = features;
  std::ofstream jf;
  jf.open(DATA_JSON_PATH);
  jf<<datas;
  jf.close();
  cv::imwrite(DATA_JSON_JPG, image3);

  Dataset dataset;

  // CONTROLLARE
//  dataset.saveGesture(datas, "./data.json");

  // #define DATASET_DIR "\"../dataset\""
  // #define DATASET_SVM "../dataset/dataset"
  // #define DATASET_SVM_MODEL "../dataset/dataset.model"
  // #define SCALING_MODEL 0
  // #define DATASET_SVM_MODEL_SCALED "../dataset/dataset.scale.model"
  // #define DATASET_PATH "../dataset/dataset.json"
  // #define TESTSET_SVM "../dataset/testset"
  // #define OUTPUT_PROBABILISTIC_SVM "../dataset/prob.khr"

  // genSVMDataset() (in Dataset.cpp)
  ifstream rdfile;
  ofstream dfile;

  rdfile.open("./data.json");
  dfile.open("./testset", std::ofstream::out | std::ofstream::app);
  std::string sline;
  nlohmann::json jline;
  while(std::getline(rdfile,sline)){
      cout << sline << endl;
//      jline = json::parse(sline);
//       dfile << jline["label"]<<" ";
//       for (int i = 0; i < jline["samples"].size(); i++) {
// //        dfile<<i+1<<":"<<jline["samples"].at(i)<<" ";
//         cout<<i+1<<":"<<jline["samples"].at(i)<<" ";
//       }
//       dfile << std::endl;
  }
  dfile.close();
  rdfile.close();
//  std::cout<<"Before svm_train, remember to order dataset; paste in terminal:\n\t> cd ../dataset\n\t> sort_dataset"<<std::endl;

  gesture_prediction("./testset", "./dataset.model", "./prob.khr");

  return 0;
}
