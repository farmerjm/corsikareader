
#include <iostream>
#include "corsikareader.h"
#include <unistd.h>
#include "TH1.h"
#include "TCanvas.h"


int main() {
  TH1D histoMuons("histoMuons","histoMuons", 100, 0, 80);
  TH1D histoMuonZ("histZ","histZ",100,0,25 );

  auto files = CorsikaFile::FetchShowersFromDir("/data/auger/MidwayCorsikaOut/0_magon/dat/");
  std::cout << "Found " << files.size() << " files."<<std::endl;


  unsigned int nFilesToRead=100;
  unsigned int nFilesRead=0;
  for (auto file : files) {
    CorsikaFile theFile(("/data/auger/MidwayCorsikaOut/0_magon/dat/"+file).c_str());

    constexpr double corrFactor=sin(55*3.1415926535897/180);
    constexpr double corrFactor2=cos(55*3.1415926535897/180);
    while (theFile.ReadNewShower()) {
      auto shower = theFile.GetShower();
      auto parts = shower.GetParticleList();
      for (auto part : parts) {
        if (part->IsMuonic()) {
          float d = sqrt(part->x*part->x+part->y*part->y);
          //if (abs(450-d) < 50) {
            histoMuons.Fill(part->GetEnergy(), part->weight);
            //histoMuonZ.Fill(part->z/cF/1E3,part->weight);
            histoMuonZ.Fill(part->z/corrFactor2/1E3,part->weight);
          //}
        }
      }
    }
    nFilesRead++;
    if (nFilesRead>nFilesToRead) break;
  }
{
  TCanvas c1;
  histoMuons.Draw();
  c1.SaveAs("histoMuons.png");}
  {TCanvas c1;
  histoMuonZ.Draw();
  c1.SaveAs("histoMuonZ.png");
  }

}
