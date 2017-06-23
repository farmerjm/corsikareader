#include <iostream>
#include "corsikareader.h"


int main() {
  CorsikaFile theFile("DAT_AUGER_NOTHIN", 0);
  theFile.Read();
  auto parts = theFile.GetParticleList();
  std::cerr << "numPart: " << parts.size();
}
