#ifndef cread
#define cread

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "corsikaparticle.h"
#include "crshower.h"


//Basic C++ implementation of a corsika filereader.
//
//Usage:  First, instantiate a CorsikaFile object:
//  CorsikaFile file(fileName, isThinned);
//Then, call CorsikaFile::ReadNewShower:
//  file.ReadNewShower();
//
//This will populate a vector of particles with id, obslevel, x, y, z, px, py, pz, etc.  You can get vector by then calling:
//  file.GetParticleList();
//
//
//ReadNewShower will return 1 unless the end of file is reached, so a shower file with multiple showers can be read by doing:
//
//while (file.ReadNewShower()) {
//  auto parts = file.GetParticleList();
//  for (auto part : parts) { 
//    //operate on particles
//  }
//}
class CorsikaFile {
    
  public:
    
    CorsikaFile(std::string name, bool _isThinned=1);
    ~CorsikaFile();
    
    const CRShower& GetShower() {return shower;}
    bool ReadNewShower();
    void SetRecoveryMode(bool r) {recoveryMode=r;}
    
  private:
    bool isThinned;
    bool recoveryMode;
    
    int blockSize;
    int subBlockSize;
    int numSubBlocks;

    unsigned int showerCount;
    unsigned int blocksRead;
    
    std::string fName;
    
    bool ReadNewBlock();
    std::string GetSubBlockType();
    bool ReadDataSubBlock();
    void ReadEventHeader();
    void RecoverShowers();

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

    CRShower shower;

};


#endif
