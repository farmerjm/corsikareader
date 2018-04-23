#pragma once
#include "TH2.h"
#include <string>
#include "corsikareader.h"
#include "TStyle.h"
#include "TCanvas.h"

class MuonMap {
private:
  bool doShowerTransversePlane;
  TH2F theMap;
  const std::string outputName;
public:
  MuonMap(std::string name);

  void AddShower(const CRShower& show);
  void Print(std::string outputName);
  
  static TH2F MakeMapFromFolder(std::string folderPath, std::string outputName);
  static TH2F MakeMapFromFolders(std::vector<std::string> folderPath, std::string outputName);
};
