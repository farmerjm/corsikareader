#ifndef cread
#define cread

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include "corsikaparticle.h"


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


#endif
