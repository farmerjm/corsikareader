
struct Ring {
  std::vector<double> corrections;
  double r

  Ring(double _r) : r(_r)  {}

  bool IsRInRing(double rIn) {return abs(r-rIn) < 100;}
  double GetRingCorrection() {
    return std::accumulate(corrections.begin(), corrections.end(), 0.0)/static_cast<double>(corrections.size());
  }
  
};

void radialBins() {
  TFile file("bigdiff_0_tri_coarse_muon.root");
  TH2F* diffMap;
  file.GetObject("muondiff",diffMap);

  const int nBins=300;
/*
  std::vector<Ring*> rings;
  for (int i=0;i<8;i++) {
    Ring newRing = new Ring(50+i*100);
    rings.push_back(newRing);
  }
*/

  std::vector<double> ringR;
  std::vector<double> ringData[10];
  for (int i=0;i<10;i++) {
    ringR.push_back(50+i*100);
  }

  for (int i=0;i<nBins;i++) {
    for (int j=0;j<nBins; j++) {
      const double x = diffMap->GetXaxis()->GetBinCenter(i+1);
      const double y = diffMap->GetYaxis()->GetBinCenter(j+1);
      const double r = sqrt(x*x+y*y);

      for (int k=0;k<10;k++) {
        if (fabs(r-ringR[k]) < 50) {
          ringData[k].push_back(diffMap->GetBinContent(i+1,j+1));
        }
      }
    }
  }

  double corrs[10];

  for (int i=0;i<10;i++) {
    corrs[i]=0;
    for (int j=0;j<ringData[i].size(); j++) {
      corrs[i]+= ringData[i][j];
    }
    corrs[i]=corrs[i]/ringData[i].size();
  }
  for (int k=0;k<10;k++) {
    std:::cout << "RING " << k*100+50 << " " << corrs[k] << std::endl;
  }

  TGraph tg;
  for (int i=0;i<10;i++) {
    tg.SetPoint(i,50+i*100, corrs[i]);
  }
  TCanvas c1;
  tg.Draw("AC");
  tg.SetLineColor(4);
  tg.SetMarkerStyle(8);
  tg.SetMarkerColor(8);
  tg.SetTitle("Average #Delta#rho(#mu) in annulus ");
  tg.GetXaxis()->SetTitle("Core distance [m]");
  tg.GetYaxis()->SetTitle("Percent change");
  c1.SaveAs("corrRings2_tri.png");
}
