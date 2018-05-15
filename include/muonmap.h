#pragma once
#include "TH2.h"
#include <string>
#include "corsikareader.h"
#include "TStyle.h"
#include "TCanvas.h"

class MuonMap {
private:
  bool isInclined;
  float inclinedAngle;
  TH2F theMap;
  double offset;
  const std::string outputName;
public:
  MuonMap(std::string name);


  void AddShower(const CRShower& show);
  void Print(std::string outputName);
  void SetInclinedAngle(float angle);
  void SetOffset(double _offset) {offset=_offset;}
  void CenterOnCore();

  TH2F* GetMap() {return &theMap;}
  
  static TH2F MakeMapFromFolder(std::string folderPath, std::string outputName,double offset=0);
  static TH2F MakeMapFromFolders(std::vector<std::string> folderPath, std::string outputName);
};
