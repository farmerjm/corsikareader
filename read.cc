#include <iostream>
#include "corsikareader.h"
#include <unistd.h>
#include "TH1.h"
#include "TGraph.h"
#include <numeric>
#include "TCanvas.h"
#include <assert.h>


int main(int argc, char* argv[]) {
  /*
  std::string folderName=argv[1];

  auto index=folderName.find_last_of('/');

  auto plotName="output"+folderName.substr(index)+".png";
  std::cout << plotName << std::endl;
  folderName+="/";

  auto showerFiles = CorsikaFile::FetchShowersFromDir(folderName+"/dat/");
  std::cout << "Found " << showerFiles.size() << " files" << std::endl;
  
  TH1D depthHisto("depthHisto","depthHisto",1000, 5E5, 1E7);
  std::vector<std::vector<double> > azBins;

  constexpr double fortyFiveDegToRad=45*3.141592653589/180;
  for (auto showerFileName : showerFiles) {
    CorsikaFile cf(folderName+"/dat/"+showerFileName,1);
    while (cf.ReadNewShower()) {
      auto shower = cf.GetShower();
      //std::cout << "bx bz e: " << shower.bx << " " << shower.bz << " " << shower.energy << " " << shower.azimuth*180/3.14159265358 << " " << shower.zenith*180/3.14159265358  << " " << shower.heightFirstInteraction << " " << shower.thinLevel <<std::endl;
      //assert(static_cast<int>(10*shower.bx*1E10)==191);
      //assert(static_cast<int>(10*shower.bz*1E10)==-141);
      depthHisto.Fill(shower.heightFirstInteraction);
      const double azimuth=shower.azimuth;
      for (int i=0;i<8;i++) {
        if (azimuth > fortyFiveDegToRad*i && azimuth < fortyFiveDegToRad*(i+1)) {
          azBins[i].push_back(shower.energy);
        }
      }
    }
  }
  {
  TCanvas c1;
  depthHisto.Draw();
  c1.SaveAs(plotName.c_str());
  }
  TGraph tg;
  for (int i=0;i<8;i++) {
    const double mean = std::accumulate(azBins[i].begin(), azBins[i].end(), 0.0);
    tg.SetPoint(i,45*i+(45./2.), mean);
  }

  TCanvas c2;
  tg.Draw();
  c2.SaveAs("meow.png");
  */

  
  
  std::string file = argv[1];

  CorsikaFile cf(file,1);


  while (cf.ReadNewShower()) {  
    auto shower = cf.GetShower();
      std::cout << shower.bx << " " << shower.bz << std::endl;
  }
}
