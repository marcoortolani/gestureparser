#ifndef FeaturesExtraction_hpp
#define FeaturesExtraction_hpp

#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <string>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class FeaturesExtraction{
  public:
    bool acquireframe(int dita, int index);
    cv::Mat edge();
    cv::Mat im2bw();
    void setHandProperties();
    void gen_distances();
    void sample_features();
    void genFeatures(int dita, int index, std::ofstream &file);
  private:
    std::vector<int> find(std::vector<int> vec, int value);
    cv::Mat frame;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Point> max_area_contour;
    std::vector<cv::Vec4i> hierarchy;
    float centroide_x;
    float centroide_y;
    float area;
    std::vector<float> distances;
    std::vector<float> features;
};

#endif /*FeaturesExtraction_hpp*/
