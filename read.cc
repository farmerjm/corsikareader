#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main() {
  int count=0;
  std::vector<std::string> theFiles = CorsikaFile::FetchShowers("bigproton_out");
  std::vector<std::string> theGoodFiles;
  unsigned int nShowers=0;
  unsigned int deltaShowers;
  unsigned int index=0;
  for (auto file : theFiles) { 
    std::cout << "Opening file " << index << " of " << theFiles.size() << "..." << std::endl;
    CorsikaFile theFile(file, 1);
    deltaShowers=0;
    while (theFile.ReadNewShower()) { 
      deltaShowers++;
      auto shower = theFile.GetShower(); 
      std::cout << shower.azimuth << std::endl;
    }
    if (!theFile.isFailure) {
      theGoodFiles.push_back(file);
      nShowers+=deltaShowers;
    }
    index++;
  }
  std::ofstream fout("goodFiles_bigproton");
  fout << nShowers << " showers; " << "nGoodFiles: " << theGoodFiles.size() << std::endl;
  std::cout << nShowers << " " << theGoodFiles.size() << std::endl;
  for (auto file : theGoodFiles) fout << file << std::endl;
}
