#include <iostream>
#include "corsikareader.h"
#include "muonmap.h"
#include <vector>
#include <string>
#include <unistd.h>


int main() {

  //std::string file = "/data/farmer/inclinetest/DAT001000.inclined";

  auto map_off = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/180_magoff/dat/", "180_magoff.png",-1600);
  auto map_tri = MuonMap::MakeMapFromFolder("/data/auger/MidwayCorsikaOut/180_magtri/dat/", "180_magtri.png",-1600);

  

}
