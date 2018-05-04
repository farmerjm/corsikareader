#include "muonmap.h"

MuonMap::MuonMap(std::string name) :
  outputName(name) {
    theMap = TH2F(name.c_str(), name.c_str(), 250, -1200, 1200, 200, -1200, 1200);
    theMap.GetXaxis()->SetTitle("X [m]");
    theMap.GetYaxis()->SetTitle("Y [m]");
    theMap.SetTitle("Muon Map");
    gStyle->SetPalette(51);
    gStyle->SetOptStat(0);
}


TH2F MuonMap::MakeMapFromFolder(std::string inFolder, std::string outputName) {
  MuonMap theMap(outputName);
  auto showerfiles = CorsikaFile::FetchShowersFromDir(inFolder); 

  for (auto showerfilename : showerfiles) {
    CorsikaFile cFile(inFolder+showerfilename, 1);
    while (cFile.ReadNewShower()) {
      auto shower =cFile.GetShower(); 
      theMap.AddShower(shower);
    }
  }
  theMap.Print(outputName);
  return theMap.theMap;
}

TH2F MuonMap::MakeMapFromFolders(std::vector<std::string> inFolders, std::string outputName) {

  const unsigned int nFilesToRead=20;

  MuonMap theMap(outputName);
  for (auto inp: inFolders) {
    std::cout << "folder: " << inp << std::endl;
    auto showerfiles = CorsikaFile::FetchShowersFromDir(inp);
    unsigned int filesRead=0;
    for (auto showerfilename : showerfiles) {
      if (filesRead >nFilesToRead) break;
      CorsikaFile cFile(inp+showerfilename, 1);
      while (cFile.ReadNewShower()) {
        auto shower =cFile.GetShower(); 
        theMap.AddShower(shower);
      }
      filesRead++;
    }
  }
  theMap.Print(outputName);
  return theMap.theMap;
}


void MuonMap::AddShower(const CRShower& show) {
  auto particleList=show.GetParticleList();
  std::cout << "NPart:" << particleList.size() << std::endl;
  for (auto particle : particleList) {
    if (particle->IsMuonic()) {
        theMap.Fill(particle->x, particle->y);
        std::cout << particle->x << " " << particle->y << std::endl;
    }
  }
}

void MuonMap::Print(std::string outputName) {
  TCanvas c1;

  theMap.Draw("colz");
  c1.SaveAs(outputName.c_str());
}
