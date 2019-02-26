#include "FeaturesExtraction.hpp"

void FeaturesExtraction::genFeatures(int dita, int index, std::ofstream &file){
  acquireframe(dita, index);
  //std::cout << "acquisico frame" << '\n';
  //im2bw();
  //std::cout << "bianco e nero" << '\n';
  //edge();
  //std::cout << "edge" << '\n';
  setHandProperties(false);
  //std::cout << "setHandProperties" << '\n';
  gen_distances();
  //std::cout << "gen_distances" << '\n';
  sample_features();
  //std::cout << "sample_features" << '\n';
  file<< dita << " ";
  for (int i=0; i<(int)features.size(); i++){
    //std::cout << i+1 << ":"<<features.at(i) << " ";
    file << i+1 << ":"<<features.at(i) << " ";
  }
  file << "\n";
}

bool FeaturesExtraction::acquireframe(int dita, int index){
  cv::Mat image;
  char integer_string[20]="";
  char percorso[50]="";
  strcat(percorso, "../dataset/G");
  sprintf(integer_string, "%d", dita);
  strcat(percorso, integer_string);
  strcat(percorso, "/");
  sprintf(integer_string, "%d", index);
  strcat(percorso, integer_string);
  strcat(percorso, "-cont.bmp");
  //std::cout << percorso << '\n';
  try{
    image= cv::imread(percorso, 0);
    this->frame=image;
    return true;
  } catch(const std::exception& e){
            std::cout << "Errore acquisizione immagine: "<<e.what()<< std::endl;
            return false;
          }
}

cv::Mat FeaturesExtraction::edge(){
    cv::Mat edged = this->frame.clone();
    cv::GaussianBlur(edged, edged, cv::Size(7,7), 1.5, 1.5);
    cv::Canny(edged, edged, 0, 120, 3);
    return edged;
}

cv::Mat FeaturesExtraction::im2bw(){
    double minVal=-1;
    double maxVal=-1;
    cv::Mat dst;
    cv::Mat temp = frame.clone();
    temp.setTo(255, temp == 0);
    cv::minMaxIdx(temp, &minVal, &maxVal);
    int delta = 10 - ((int)round(minVal) % 10);
    cv::inRange(frame, cv::Scalar(minVal), cv::Scalar(minVal+delta), dst);
    this->frame = dst.clone();
    return dst;
}


void FeaturesExtraction::setHandProperties(bool perimetro){
    cv::Moments moment;
    int index_max_area, index_max_perimetro, temp_size, size_;
    findContours(frame,contours,hierarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE,cv::Point(0,0));
    index_max_area = 0;
    size_=0;
    temp_size=0;
    if (perimetro){
      for(int i=0; i< (int)contours.size(); i++){
        temp_size=contours.at(i).size();
        if(temp_size>size_)
          index_max_perimetro=i;
      }
      moment = moments((cv::Mat)contours[index_max_perimetro]);
      this->centroide_x = round(moment.m10/moment.m00);
      this->centroide_y = round(moment.m01/moment.m00);
      this->area = moment.m00;
      this->max_area_contour=contours.at(index_max_perimetro);
    } else {
      for(int i=0; i< (int)contours.size(); i++){
        moment = moments((cv::Mat)contours[i]);
        if(moments((cv::Mat)contours[index_max_area]).m00 <= moment.m00){
            index_max_area = i;
            this->centroide_x = round(moment.m10/moment.m00);
            this->centroide_y = round(moment.m01/moment.m00);
            this->area = moment.m00;
            this->max_area_contour=contours.at(index_max_area);
        }
      }
    }

}

void FeaturesExtraction::gen_distances(){
    float temp_max_value=0;
    float x_diff, y_diff, temp_dist, index_lower_point;
    int min_x = 9999;
    int min_y=0;
    std::vector<float> temp_distances;
    for(int i = 0; i < (int) this->max_area_contour.size() ; i++){
      if ((max_area_contour.at(i).x < min_x) && (max_area_contour.at(i).y > min_y)){
        index_lower_point=i;
        //std::cout << i << '\n';
      }
      x_diff=this->max_area_contour.at(i).x - centroide_x;
      y_diff=this->max_area_contour.at(i).y - centroide_y;
      x_diff=x_diff*x_diff;
      y_diff=y_diff*y_diff;
      temp_dist=sqrt(x_diff+y_diff);
      temp_distances.push_back(temp_dist);
      if (temp_dist>temp_max_value){
        temp_max_value=temp_dist;
      }
    }
    //salvo le distanze a partire dal punto in basso a sinistra
    for (int i=index_lower_point; i<(int)temp_distances.size(); i++){
      this->distances.push_back(temp_distances.at(i));
    }
    for (int i=0; i<index_lower_point; i++){
      this->distances.push_back(temp_distances.at(i));
    }
  this->distances=temp_distances;
}

std::vector<int> FeaturesExtraction::find(std::vector<int> vec, int value){
  std::vector<int> indexes;
  for (int i=0; i<(int)vec.size(); i++){
    if (vec.at(i) == value)
      indexes.push_back(i);
  }
  return indexes;
}

void FeaturesExtraction::sample_features(){
  std::vector<int> indexes, temp_ind;
  float costante=0;
  float temp_val=0;
  for (int i=0; i<=(int)distances.size(); i++){
    indexes.push_back(i);
  }
  costante=(float)(200/(float)distances.size());  //il vettore finale sarà di 200 campioni
  //std::cout << "costante= " << costante <<'\n';
  for (int i=0; i<=(int)distances.size(); i++){
    indexes.at(i)=ceil(indexes.at(i)*costante);
  }
  //sottocampiono
  if (costante <= 1) {
    for (int i=0; i<200; i++){
      temp_val=0;
      temp_ind=find(indexes, i);
      for (int j=0; j<(int)temp_ind.size(); j++)
          temp_val=temp_val+distances.at(temp_ind.at(j));
      temp_val=temp_val/temp_ind.size();
      this->features.push_back(temp_val);
    }
  } else {  //sovracampiono
      features.push_back(distances.at(0));
      for (int i=1; i<200; i++){
        temp_ind=find(indexes, i);
        int j=i;
        while (temp_ind.size() == 0){
          j=j+1;
          temp_ind=find(indexes, j);
        }
        if (i==j){
              this->features.push_back(distances.at(temp_ind.at(0)));
        } else {
              this->features.push_back((features.at(i-1)+distances.at(temp_ind.at(0)-1))/2);
        }
      }
    }
}
