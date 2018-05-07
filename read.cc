#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main(int argc, char* argv[]) {
  std::string fileName=argv[1];
  
  CorsikaFile cf(fileName);

  while (cf.ReadNewShower()) {
    auto shower = cf.GetShower();
    std::cout << "bx bz: " << shower.bx << " " << shower.bz << std::endl;
  }
}
