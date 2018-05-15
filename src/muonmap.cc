#include "muonmap.h"

MuonMap::MuonMap(std::string name) :
  outputName(name) {
    isInclined=0;
    theMap = TH2F(name.c_str(), name.c_str(), 200, -500, 500, 200, -500, 500);
    theMap.GetXaxis()->SetTitle("X [m]");
    theMap.GetYaxis()->SetTitle("Y [m]");
    theMap.SetTitle("Muon Map");
    gStyle->SetPalette(51);
    gStyle->SetOptStat(0);
}


TH2F MuonMap::MakeMapFromFolder(std::string inFolder, std::string outputName, double offset) {
  MuonMap theMap(outputName);
  theMap.SetOffset(offset);
  auto showerfiles = CorsikaFile::FetchShowersFromDir(inFolder); 

  const unsigned int nFilesToRead=114;
  unsigned int filesRead=0;

  for (auto showerfilename : showerfiles) {
    if (filesRead>=nFilesToRead) break;
    CorsikaFile cFile(inFolder+showerfilename, 1);
    while (cFile.ReadNewShower()) {
      auto shower =cFile.GetShower(); 
      theMap.AddShower(shower);
    }
    filesRead++;
  }
  theMap.Print(outputName);
  return theMap.theMap;
}

void MuonMap::CenterOnCore() {
  auto maxBin = theMap.GetMaximumBin();
  TH2F centeredMap("centeredmap","centeredmap", 200, -500, 500, 200, -500, 500);
  centeredMap.GetXaxis()->SetTitle("X [m]");
  centeredMap.GetYaxis()->SetTitle("Y [m]");
  centeredMap.SetTitle("Muon Map");
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

void MuonMap::SetInclinedAngle(float angle) {
  isInclined=1;
  inclinedAngle=angle/180*3.14159265358979323;
}


void MuonMap::AddShower(const CRShower& show) {
  auto particleList=show.GetParticleList();
  std::cout << "NPart:" << particleList.size() << std::endl;
  for (auto particle : particleList) {
    if (particle->IsMuonic()) {
        theMap.Fill(particle->x+offset, particle->y,particle->weight);
        //std::cout << particle->x+1400 << " " << particle->y << std::endl;
    }
  }
}

void MuonMap::Print(std::string outputName) {
  TCanvas c1("can","can",0,0,800,800);

  theMap.Draw("colz");
  c1.SaveAs(outputName.c_str());
}
