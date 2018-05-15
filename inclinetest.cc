#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {

  std::string file = "/data/farmer/inclinetest/DAT001000.inclined";
  //std::string file = "/data/auger/MidwayCorsikaOut/180_magon/dat/DAT000001.inclined";


  MuonMap theMap("inclinetest");
  theMap.SetOffset(-1600);
  CorsikaFile cf(file);

  while (cf.ReadNewShower()) {
    std::cout << "Reading shower..." << std::endl;
    auto shower=cf.GetShower();
    theMap.AddShower(shower);
  }

  theMap.Print("inclinetest.png");

}
