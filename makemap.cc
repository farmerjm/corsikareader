#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {
  std::string nomag[8] = {"geomag_az42.4", "geomag_az87.4", "geomag_az132.4", "geomag_az177.4", "geomag_az222.4", "geomag_az267.4", "geomag_az312.4", "geomag_az357.4"};
  std::string mag[8] = {"geomag_azmag42.4", "geomag_azmag87.4", "geomag_azmag132.4", "geomag_azmag177.4", "geomag_azmag222.4", "geomag_azmag267.4", "geomag_azmag312.4", "geomag_azmag357.4"};

  std::vector<std::string> nomagvec;
  std::vector<std::string> magvec;
  std::string head = "/data/auger/MidwayCorsikaOut/";
  for (int i=0; i<8; i++) {
    nomagvec.push_back(head+nomag[i]+"/dat/");
    magvec.push_back(head+mag[i]+"/dat/");
  }

  for (int i=0;i<8; i++) {
    std::cout << "i="<<i<<std::endl;
    TH2F magbin = MuonMap::MakeMapFromFolder(magvec[i], Form("maps/mag%i.png", i));
    TH2F nomagbin = MuonMap::MakeMapFromFolder(nomagvec[i], Form("maps/nomag%i.png", i));
    TH2F diffMapBin = TH2F(Form("diff%i",i), Form("diff%i",i), 250, -1200, 1200, 200, -1200, 1200);
    diffMapBin.GetXaxis()->SetTitle("X [m]");
    diffMapBin.GetYaxis()->SetTitle("Y [m]");
    diffMapBin.SetTitle("Muon Map Difference [%]");
    gStyle->SetPalette(51);
    gStyle->SetOptStat(0);

    diffMapBin.Add(&magbin, &nomagbin, 100, -100);
    diffMapBin.Divide(&magbin);
    
    TCanvas c1;
    diffMapBin.Draw("colz");
    c1.SaveAs(Form("diffmap%i.png",i));
  }
  return 2;

  std::vector<std::string> testVec;
  testVec.push_back(head+"geomag_az42.4"+"/dat/");
  MuonMap::MakeMapFromFolders(testVec, "testmag.png");
  TH2F nomagMap = MuonMap::MakeMapFromFolders(nomagvec, "nomag.png");
  TH2F magMap = MuonMap::MakeMapFromFolders(magvec, "mag.png");
  
  TH2F diffMap = TH2F("diff", "diff", 250, -1200, 1200, 200, -1200, 1200);
  diffMap.GetXaxis()->SetTitle("X [m]");
  diffMap.GetYaxis()->SetTitle("Y [m]");
  diffMap.SetTitle("Muon Map Difference [%]");
  gStyle->SetPalette(51);
  gStyle->SetOptStat(0);

  diffMap.Add(&nomagMap, &magMap, 100, -100);
  diffMap.Divide(&magMap);
  
  TCanvas c1;
  diffMap.Draw("colz");
  c1.SaveAs("diffmap.png");
}
