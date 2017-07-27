#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main() {
  CorsikaFile theFile("DAT_AUGER_NOTHIN", 0);
  int count=0;
  while (theFile.ReadNewShower()) { 
    count++;
  }
  std::cout << "Number of showers read: " << count << std::endl;
}
