#include <iostream>
#include "corsikareader.h"


int main() {
  CorsikaFile theFile("DAT_AUGER_NOTHIN", 0);
  theFile.Read();
}
