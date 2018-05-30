#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>
#include "TFile.h"


int main() {
  /*
  std::string noMagParallel = "/data/farmer/inclinetest/DAT000100.inclined";
  std::string strParallelMag="/data/farmer/inclinetest/DAT000103.inclined";
  
  double theOffset=1800;
  */

  MuonMap::InvertMap(0);

  auto ParallelMag = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/0_magtri/dat/", "0_magtri.png",2285);
  auto NoMagParallel = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/0_magoff/dat/", "0_magoff.png",2285);

  //auto ParallelMag = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/180_magon/dat/", "180_magon.png",-2300);
  //auto NoMagParallel = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/180_magoff/dat/", "180_magoff.png",-2300);
  
  const int nBins=300;

  TH2F* diffMap = new TH2F("diffmap","diffmap", nBins, -1000, 1000,nBins, -1000, 1000);
  diffMap->GetXaxis()->SetTitle("X [m]");
  diffMap->GetYaxis()->SetTitle("Y [m]");
  diffMap->SetTitle("Muon Map");
  
  std::cout << "Adding maps..." << std::endl;

  
  std::vector<double> percentDiffsInRing;
  for (int i=0; i<nBins; i++) {
    for (int j=0; j<nBins;j++) {
      const double withMag = ParallelMag.GetBinContent(i+1,j+1);
      const double noMag = NoMagParallel.GetBinContent(i+1,j+1);
      const double percentDiff = (withMag-noMag)/noMag*100;
      //if ((withMag-noMag)/noMag > 5) diffMap->SetBinContent(i+1,j+1, 0); else  diffMap->SetBinContent(i+1,j+1, (withMag-noMag)/noMag*100);
      diffMap->SetBinContent(i+1,j+1, percentDiff);

      const double x = diffMap->GetXaxis()->GetBinCenter(i+1);
      const double y = diffMap->GetYaxis()->GetBinCenter(j+1);
      const double r = sqrt(x*x+y*y);

      //std::cout << i << " " << j << " " << withMag << " " << noMag << " " << percentDiff << std::endl;

      if (fabs(450-r) < 50) percentDiffsInRing.push_back(percentDiff);
      if (isnan(percentDiff)) std::cout << "NaN: " << i << " " << j << " " << withMag << " " << noMag << std::endl;
    }
  }

  double runningSum=0;
  for (int i=0; i<percentDiffsInRing.size(); i++) {
    runningSum+=percentDiffsInRing[i];
  }
  std::cout << "nCorrections: " << percentDiffsInRing.size() << std::endl;

  runningSum=runningSum/percentDiffsInRing.size();

  std::cout << "Average correction: " << runningSum << std::endl;
  std::cout << "Done." << std::endl;


  TCanvas c1;
  std::cout << "made can" << std::endl;
  diffMap->Draw("colz");
  if (MuonMap::IsInverted()) diffMap->SetTitle("Electromagnetic Map"); 
  std::cout << "Drew" << std::endl;
  c1.SaveAs("bigdiff_0_tri_coarse_muon.png");
  TFile outFile("bigdiff_0_tri_coarse_muon.root", "NEW");
  diffMap->Write("muondiff");
  std::cout << "Finished!" << std::endl;

}
