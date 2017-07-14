#include <iostream>
#include "corsikareader.h"
#include <unistd.h>


int main() {
  std::cout << "float char: " << sizeof(float) << " " << sizeof(char) << " " << sizeof(double) << std::endl;
  CorsikaFile theFile("10k1E12", 1);
  int count=0;
  while (theFile.ReadNewShower()) { 
    count++;
  }
  std::cout << "Number of showers read: " << count << std::endl;
}
