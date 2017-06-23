#ifndef cread
#define cread

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "corsikaparticle.h"

class CorsikaFile {
    
  public:
    
    CorsikaFile(std::string name, bool _isThinned=1);
    ~CorsikaFile();
    
    std::vector<CorsikaParticle*> Read();
    
  private:
    bool isThinned;
    
    int blockSize;
    int subBlockSize;
    int numSubBlocks;
    
    char* fName;
    
    static constexpr int blockSize_unthinned=22932;
    static constexpr int blockSize_thinned=26208;
    static constexpr int subBlockSize_unthinned=273;
    static constexpr int subBlockSize_thinned=312;

};


/*int CorsikaFile::blockSize_unthinned;
int CorsikaFile::blockSize_thinned;
int CorsikaFile::subBlockSize_unthinned;
int CorsikaFile::subBlockSize_unthinned;*/

#endif
