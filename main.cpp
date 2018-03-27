#include <unistd.h>

#include "hand-recognizer.h"

using namespace std;
using namespace cv;

int main(int argc, char* argv[]) {
  // Simulaiamo la connessione con la kinect.
  Dataset dataset;
//  InputGenerator command_exec;
  KinectDevice kinect;

  string imgPath = "../dataset/dx/0/1481713432723.jpg";
  cout << "Reading in image: " << imgPath << endl;

  Mat image = cv::imread(imgPath, -1 );

  cv::imshow("Pugno chiuso", image);
  cv::waitKey(0);

  Mat img2;
  image.convertTo( img2, CV_8UC1, kinect.getScaleFactor() );

  double minVal=-1;
  double maxVal=-1;
  cv::Mat dst;
  cv::Mat temp = kinect.getFrame().clone();
  temp.setTo(255, temp == 0);
  cv::minMaxIdx(temp, &minVal, &maxVal);
  int delta = 10 - ((int)round(minVal) % 10);
  cv::inRange(kinect.getFrame(), cv::Scalar(minVal), cv::Scalar(minVal+delta), dst);

  cv::imshow("Pugno chiuso 2", img2);
  cv::waitKey(0);

  cout << "Width : " << img2.cols << endl;
  cout << "Height: " << img2.rows << endl;

  kinect.setFrame(img2);
  // frame=image;
//  cv::imwrite("ciccio", kinect.im2bw());
  cv::imwrite("ciccio3.jpg", kinect.getFrame());

  return 0;
}
