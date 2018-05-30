
#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {

  std::string file = "/data/auger/MidwayCorsikaOut/inclinetest/DAT000001.inclined";

  TH2F map = MuonMap::MakeMapFromFolder
    diffMapBin.GetXaxis()->SetTitle("X [m]");
    diffMapBin.GetYaxis()->SetTitle("Y [m]");
    diffMapBin.SetTitle("Muon Map Difference [%]");
    gStyle->SetPalette(51);
    gStyle->SetOptStat(0);

    MuonMap map("inclineTest");
    CorsikaFile cf(file);
    while (cf.ReadNewShower()) {
      auto shower = cf.GetShower();
      map.addShower(shower);
    }


}
