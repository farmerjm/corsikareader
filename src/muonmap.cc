#include "muonmap.h"

bool MuonMap::invertMap;

MuonMap::MuonMap(std::string name) :
  outputName(name) {
    isInclined=0;
    offset=0;
    theMap = TH2F(name.c_str(), name.c_str(), 300, -1000, 1000, 300, -1000, 1000);
    theMap.GetXaxis()->SetTitle("X [m]");
    theMap.GetYaxis()->SetTitle("Y [m]");
    if (invertMap) theMap.SetTitle("Electromagnetic Map"); else 
      theMap.SetTitle("Muon Map");
    gStyle->SetPalette(51);
    gStyle->SetOptStat(0);
}


TH2F MuonMap::MakeMapFromFolder(std::string inFolder, std::string outputName, double offset) {
  MuonMap theMap(outputName);
  theMap.SetOffset(offset);
  auto showerfiles = CorsikaFile::FetchShowersFromDir(inFolder); 

  const unsigned int nFilesToRead=3000;
  unsigned int showersRead=0;
  unsigned int filesRead=0;

  for (auto showerfilename : showerfiles) {
    if (filesRead>=nFilesToRead) break;
    CorsikaFile cFile(inFolder+showerfilename, 1);
    while (cFile.ReadNewShower()) {
      auto shower =cFile.GetShower(); 
      theMap.AddShower(shower);
      showersRead++;
    }
    filesRead++;
  }
  //std::cout << filesRead << " maps read from " << inFolder << std::endl;
  auto norm = static_cast<double>(showersRead);
  for (int i=0; i<300; i++) {
    for (int j=0; j<300;j++) {
      theMap.theMap.SetBinContent(i+1,j+1, theMap.theMap.GetBinContent(i+1,j+1)/norm);
    }
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
  for (auto particle : particleList) {
    if (!invertMap) {
      if (particle->IsMuonic()) 
          theMap.Fill(particle->x*tan(55*3.14159/180)+offset, particle->y,particle->weight);
    } else if (invertMap) {
      if (particle->IsElectromagnetic()) 
          theMap.Fill(particle->x*tan(55*3.14159/180)+offset, particle->y,particle->weight);
    }
  }
}

void MuonMap::Print(std::string outputName) {
  TCanvas c1("can","can",0,0,800,800);

  theMap.Draw("colz");
  c1.SaveAs(outputName.c_str());
}
