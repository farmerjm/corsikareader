#include <iostream>
#include "corsikareader.h"
#include <unistd.h>
#include "TH1.h"
#include "TGraph.h"
#include <numeric>
#include "TCanvas.h"
#include <assert.h>


int main(int argc, char* argv[]) {
  std::string dirName="/data/auger/MidwayCorsikaOut/new/geomag_azmag360/dat/";
  auto showerFiles = CorsikaFile::FetchShowersFromDir(dirName);
  std::cout << "cherce " << dirName << " trouve " << showerFiles.size() << std::endl; 

  TH1D histoZ("hZ", "hZ", 4000, 0,40);
  TH1D hDot("hDot", "hDot", 100, 0, 1);
  TH1D hA("hA", "hA", 150, 0, 1500);


  constexpr double corrFactor=cos(55*3.1415926/180);

  const int nFilesToRead=500;
  int nFilesRead=0;
  for (auto showerFileName : showerFiles) {
    CorsikaFile cf(dirName+showerFileName);
    while (cf.ReadNewShower()) {
      auto shower = cf.GetShower();
      const double azimuthShower=shower.azimuth;
      const double zenithShower= shower.zenith;
      auto partList = shower.GetParticleList();
      for (auto part : partList) {
        if (part->IsMuonic()) {
          const double p = sqrt(part->px*part->px+part->py*part->py+part->pz*part->pz)*1000; //Momentum in GeV

          const double pAzimuth=atan2(part->py, part->px);
          const double pZenith=acos((part->pz*1000)/p);

          const double dotProduct=(sin(zenithShower)*sin(pZenith)*cos(azimuthShower-pAzimuth)+cos(zenithShower)*cos(pZenith));
          const double transverseMomentum=p*sqrt(fabs(1-dotProduct*dotProduct)); 

          //if (fabs(part->x*part->x+part->y*part->y-2500) < 202500)  {
            histoZ.Fill(part->z/corrFactor/1E3);
            hDot.Fill(transverseMomentum/p);
            hA.Fill(part->z*transverseMomentum/p); 
          //}
        }
      }
    }
    nFilesRead++;
    if (nFilesRead==nFilesToRead) break;
  }

    {TCanvas c1; histoZ.Draw(); c1.SaveAs("pmag_z.png");}
    {TCanvas c1; hDot.Draw(); c1.SaveAs("pmag_dot.png");}
    {TCanvas c1; hA.Draw(); c1.SaveAs("pmag_a.png");}
}
