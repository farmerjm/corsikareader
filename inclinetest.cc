#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {

  std::string file = "/data/auger/MidwayCorsikaOut/inclinetest/DAT000002";


  MuonMap theMap("inclinetest");
  CorsikaFile cf(file);

  while (cf.ReadNewShower()) {
    std::cout << "Reading shower..." << std::endl;
    auto shower=cf.GetShower();
    theMap.AddShower(shower);
  }

  theMap.Print("inclinetest.png");

}
