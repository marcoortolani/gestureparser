#include "hand-recognizer.h"
#include "train.h"
#include <unistd.h>

void print_menu(){
    std::cout<<"\nOpzioni\n"
            <<"1) Genera le features dal dataset\n"
            <<"2) SVM train\n"
            <<"3) Cross Validation\n"
            <<"4) Testa il modello\n\n"
            <<"9) Exit\n\n"
            <<"Input: ";
}

void genera_features(KinectDevice& kinect, Dataset& dataset, int img_number, int mode, int n_dita);

int main() {
  InputGenerator command_exec;
  KinectDevice kinect;
  Dataset dataset;
  int opt;
  int scegli_dataset, img_number;
  do {
    print_menu();
    std::cin>>opt;
    switch (opt) {
      case 1:{
        std::remove("../dataset/testset.json");
        std::remove("../dataset/testset");
        for (scegli_dataset=0; scegli_dataset<6; scegli_dataset++){
          for(img_number=1; img_number<31; img_number++)
              genera_features(kinect, dataset, img_number, 1, scegli_dataset);
        }
      std::cout << "Generato il file ./dataset/testset contenente le features" << '\n';
      //riordino le features ed elimino le prime 10 di ogni label
      system("sort -g ../dataset/testset > sorted_t");
      system("sed -i '1,90d' sorted_t");
      system("cut -f 2-122 -d' ' sorted_t > sorted_t2");
      system("sort -g sorted_t2 > ../dataset/testset");
      system("rm sorted_t sorted_t2");
      system("echo Ho riordinato il dataset tramite la bash");
      break;
      }
      case 2:{
        int risp;
        std::cout << "Quale features uso?\n"
        "0-quelle generate al punto precedente\n"
        "1-quelle nuove (Mauro)" << '\n';
        std::cin >> risp;
        if(risp==0){
          train("../dataset/testset", "../dataset/dataset_new.model",0);
          std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
        } else if (risp==1){
          train("../dataset/dataset-mauro", "../dataset/dataset_new.model",0);
          std::cout << "\nAddestrato con il nuovo dataset" << '\n';
        } else {
          train("../dataset/testset", "../dataset/dataset_new.model",0);
          std::cout << "\nAddestrato con il file generato al puno 1" << '\n';
        }
        break;
      }
      case 3:{
        int risp;
        std::cout << "Quale features uso?\n"
        "0-quelle generate al punto precedente\n"
        "1-quelle nuove (Mauro)" << '\n';
        std::cin >> risp;
        if(risp==0){
          train("../dataset/testset", "../dataset/dataset_new.model",1);
        } else if (risp==1){
          train("../dataset/dataset-mauro", "../dataset/dataset_new.model",1);
        } else {
          train("../dataset/testset", "../dataset/dataset_new.model",1);
        }
        break;
      }
      case 4:{
        int risp;
        std::cout << "Quale features uso?\n"
        "0-quelle generate al punto 1\n"
        "1-quelle nuove (Mauro)\n"
        "2-il testset(Mauro)"<< '\n';
        std::cin >> risp;
        std::remove("../dataset/prob.khr");
        if(risp==0){
          gesture_prediction("../dataset/testset","../dataset/dataset_new.model","../dataset/prob.khr");
        } else if (risp==1){
          gesture_prediction("../dataset/dataset-mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        } else {
          gesture_prediction("../dataset/testset-mauro","../dataset/dataset_new.model","../dataset/prob.khr");
        }
        break;
      }
      case 9:
          exit(1);
      default:
          std::cout<<"Invalid command.\n";
          break;
    }
  } while(1);
  return 0;
}
void genera_features(KinectDevice& kinect, Dataset& dataset, int img_number, int mode, int n_dita){
    kinect.jpgToFrame(img_number, mode, n_dita, 0); //imread
    //kinect.edge(); //già estratto nelle immagini del dataset
    kinect.extractCountours();
    kinect.setHandFeatures();
    kinect.initialValue();
    kinect.findAngle();
    kinect.normalize();
    kinect.samples();
    kinect.printSizeInfo();
    nlohmann::json test = kinect.genJSON();
    dataset.saveGesture(n_dita, test);    //genera il file ./dataset/testset
}
