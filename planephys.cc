#include <iostream>
#include "corsikareader.h"
#include <unistd.h>
#include <assert.h>
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <numeric>
#include "TStyle.h"


int main(int argc, char* argv[]) {
  std::string folderName=argv[1];
  gStyle->SetPalette(51);
  

  auto showerFiles = CorsikaFile::FetchShowersFromDir(folderName);
  std::cout << "Found " << showerFiles.size() << " files" << std::endl;
  TH1D planeMomentum("hP", "hP", 1000, 1, 1E4);

  const double pi = 3.1415926535897;
  const double zenith = 53.7074*pi/180.f;
  const double azimuth = (2.4-4)*pi/180.f;

  const double zenithShower=(125)*pi/180.f;
  const double azimuthShower=0;

  const double xHatGeomag = sin(zenith)*cos(azimuth);
  const double yHatGeomag = sin(zenith)*sin(zenith);
  const double zHatGeomag = cos(zenith);

  const double cRadToDeg=180/pi;
  TH1F hAng("hAng", "hAng", 100, 0, 360);
  TH1F hZen("hZen","hZen", 100, 0, 180);
  TH1F hAz("hAz", "hAz", 100, 0, 360);
  TH1F hDot("hDot", "hDot", 100, -1, 1);
  TH1F hRatio("hRatio","hRatio", 100,0,1);
  TH2F hPhys("hPhys","hPhys", 200, 0, 1E3,1000,0,1E6);

  const double offset=2285; 
  const double corrFactor=tan(55./180*pi);
  std::cout << "cF: " << corrFactor << std::endl;
  const unsigned int numShowers=50;
  unsigned int numRead=0; 
  std::vector< std::vector<double> > sigs;

  for (int i=0;i<12;i++) {
    sigs.push_back(std::vector<double>());
  }

  for (auto showerFileName : showerFiles) {
    CorsikaFile cf(folderName+showerFileName,1);
    while (cf.ReadNewShower()) {
      auto shower = cf.GetShower();
      auto partList = shower.GetParticleList(); 
      for (auto part: partList) {

        /* 
         * Now, get the transverse momentum.
         *
         * \vec{p}_\perp = \vec{p}-\vec{p}_\parallel
         * \implies \vec{p}_\perp = p \hat{p} - \vec{p} \hat{p} \cdot \hat \hat{H}
         * \implies p_\perp =p  p \sqrt{1- \hat{p} \cdot \hat{H}}
         */

        if (!part->IsMuonic()) continue;
        
        const double p = sqrt(part->px*part->px+part->py*part->py+part->pz*part->pz)*1000; //Momentum in GeV

        const double pAzimuth=atan2(part->py, part->px);
        const double pZenith=acos((part->pz*1000)/p);


        //const double dotProduct=-(sin(zenith)*sin(pZenith)*cos(azimuth-pAzimuth)+cos(zenith)*cos(pZenith));
        const double dotProduct=(sin(zenithShower)*sin(pZenith)*cos(azimuthShower-pAzimuth)+cos(zenithShower)*cos(pZenith));
        const double parallelMomentum=fabs(p*dotProduct);

        const double transverseMomentum=p*sqrt(fabs(1-dotProduct*dotProduct)); 

        //std::cout << "transverseMomentum, p, ratio: " << transverseMomentum << " " << p << " " << p/transverseMomentum << " " << dotProduct << std::endl;
        
        const double r = sqrt(pow(part->x*corrFactor+offset,2)+part->y*part->y);

        
        auto weight = part->weight;
        hAng.Fill(acos(dotProduct)*cRadToDeg, weight);
        hZen.Fill(pZenith*cRadToDeg, weight);
        hDot.Fill(dotProduct, weight);
        hAz.Fill(pAzimuth*cRadToDeg, weight);
        hRatio.Fill(transverseMomentum/p,weight);

        planeMomentum.Fill(transverseMomentum,weight);
        hPhys.Fill(r,transverseMomentum, weight);
      }
      numRead++;
      if ((numRead % 10) == 0) std::cout << "numRead: " << numRead << std::endl;
    }
      if (numRead>numShowers) {
        std::cout << "Breaking after reading " << numRead << " showers. " << std::endl;
        break;
      }
  }

  {
  TCanvas c1;
  hAng.Draw();
  c1.SaveAs("out.png");
  }
  {TCanvas c1; hZen.Draw(); c1.SaveAs("zen.png");}
  {TCanvas c1; hRatio.Draw(); c1.SaveAs("hRatio.png");}
  {TCanvas c1; hAz.Draw(); c1.SaveAs("Az.png");}
  {
    TCanvas c1; 
    planeMomentum.Draw(); 
    TF1* momFunc = new TF1("momFunc", "[1]*exp(-x/[0])", 800, 1E4);
    momFunc->SetParameter(0,1000);
    momFunc->SetParameter(1,3000);
    planeMomentum.Fit(momFunc,"", "", 800,1E4);
    std::cout << "P_0: " << planeMomentum.GetFunction("momFunc")->GetParameter(0) << std::endl;
    std::cout << "P_1: " << planeMomentum.GetFunction("momFunc")->GetParameter(1) << std::endl;
    c1.SaveAs("mom.png");
    }
  {TCanvas c1; hDot.Draw(); c1.SaveAs("hDot.png");}
  {
    TCanvas c1; 
    hPhys.Draw("colz"); 
    /*TGraph tg;
    for (int i=0;i<12;i++) {
      tg.SetPoint(i,100*i, means[i]);
    }
    tg.SetMarkerStyle(8);
    tg.SetMarkerColor(kRed);
    tg.Draw("CP");
    */
    TF1* func = new TF1("f1", "exp(-x/[0])", 100, 800);
    func->SetParameter(0,100);
    hPhys.Fit(func);
    std::cout << hPhys.GetFunction("f1")->GetParameter(0) << std::endl;
    
    c1.SaveAs("hPhys.png");}
}
