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
    

    const std::vector<CorsikaParticle*>&  GetParticleList() {return ParticleList;}

    bool ReadNewShower();
    
  private:
    bool isThinned;
    
    int blockSize;
    int subBlockSize;
    int numSubBlocks;
    
    std::string fName;
    
    bool ReadNewBlock();
    std::string GetSubBlockType();
    bool ReadDataSubBlock();

    float* blockBuff;
    float* subBlockBuff;
    char* sizeBuff;
    int blockIndex;
    std::string head;

    static constexpr int blockSize_unthinned=5733;
    static constexpr int blockSize_thinned=6552;
    static constexpr int subBlockSize_unthinned=273;
    static constexpr int subBlockSize_thinned=312;

    std::ifstream fin;

    std::vector<CorsikaParticle*> ParticleList;

};


/*int CorsikaFile::blockSize_unthinned;
int CorsikaFile::blockSize_thinned;
int CorsikaFile::subBlockSize_unthinned;
int CorsikaFile::subBlockSize_unthinned;*/

#endif
