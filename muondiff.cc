#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {
  /*
  std::string noMagParallel = "/data/farmer/inclinetest/DAT000100.inclined";
  std::string strParallelMag="/data/farmer/inclinetest/DAT000103.inclined";
  
  double theOffset=1800;
  */

  
  std::string noMagParallel = "/data/farmer/inclinetest/DAT000200.inclined";
  std::string strParallelMag="/data/farmer/inclinetest/DAT000203.inclined";
  double theOffset=-1800;
  
  MuonMap NoMagParallel("nomagParallel");
  NoMagParallel.SetOffset(theOffset);
  CorsikaFile cf_NoMagParallel(noMagParallel);

  while (cf_NoMagParallel.ReadNewShower()) {
    std::cout << "Reading shower..." << std::endl;
    auto shower=cf_NoMagParallel.GetShower();
    NoMagParallel.AddShower(shower);
  }

  MuonMap ParallelMag("parallelmag");
  ParallelMag.SetOffset(theOffset);
  CorsikaFile cf_ParallelMag(strParallelMag);

  while (cf_ParallelMag.ReadNewShower()) {
    auto shower = cf_ParallelMag.GetShower();
    ParallelMag.AddShower(shower);
  }

  TH2F diffMap("diffmap","diffmap", 200, -500, 500, 200, -500, 500);
  diffMap.GetXaxis()->SetTitle("X [m]");
  diffMap.GetYaxis()->SetTitle("Y [m]");
  diffMap.SetTitle("Muon Map");
  
  std::cout << "Adding maps..." << std::endl;
  for (int i=0; i<200; i++) {
    for (int j=0; j<200;j++) {
      double withMag = ParallelMag.GetMap()->GetBinContent(i+1,j+1);
      double noMag = NoMagParallel.GetMap()->GetBinContent(i+1,j+1);
      if ((withMag-noMag)/noMag > 5) diffMap.SetBinContent(i+1,j+1, 0); else  diffMap.SetBinContent(i+1,j+1, (withMag-noMag)/noMag*100);
    }
  }
  //diffMap.Add(ParallelMag.GetMap(), 1);
  //diffMap.Add(NoMagParallel.GetMap(),-1);

  std::cout << "Done." << std::endl;


  TCanvas c1;
  std::cout << "made can" << std::endl;
  diffMap.Draw("colz");
  std::cout << "Drew" << std::endl;
  c1.SaveAs("map_antipar.png");
  std::cout << "Finished!" << std::endl;

}
