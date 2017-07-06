#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main() {
  std::cout << "float char: " << sizeof(float) << " " << sizeof(char) << " " << sizeof(double) << std::endl;
  CorsikaFile theFile("DAT_AUGER_NOTHIN", 0);
  theFile.Read();
  std::cout << "cwd: " << get_current_dir_name() << std::endl;
  auto parts = theFile.GetParticleList();
  std::cerr << "numPart: " << parts.size();
}
