#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main() {
  CorsikaFile theFile("10k1E12", 1);
  int count=0;
  while (theFile.ReadNewShower()) { 
    auto shower = theFile.GetShower();
    auto partList = shower.GetParticleList();
    //for (auto part: partList) part->Dump();
  }
  std::cout << "Done reading." << std::endl;
}
