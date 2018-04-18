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
//Then, call CorsikaFile::ReadNewShower to get the std::vector<corsikaparticle*>:
//  auto partlist = file.ReadNewShower();
//
//This will create a shower object with some shower properties.  Then call:
// auto shower = theFile.GetShower();
// auto partlist = shower.GetParticleList();
//
//Now you have the particle list, containing px, py, pz, x, y, z, and weight for each particle.
//
//ReadNewShower will return 1 unless the end of file is reached, so a shower file with multiple showers can be read by doing:
//
//while (file.ReadNewShower()) {
//  auto shower = file.GetShower();
//  auto parts = shower.GetParticleList();
//  for (auto part : parts) { 
//    //do something with part->x, etc.
//  }
//}
class CorsikaFile {
    
  public:
    
    CorsikaFile(std::string name, bool _isThinned=1);
    ~CorsikaFile();
    
    const CRShower& GetShower() {return shower;}
    bool ReadNewShower();
    void SetRecoveryMode(bool r) {recoveryMode=r;}
    void ReserveNParticles(unsigned int n) {CRShower::ReserveN=n;}

    static std::vector<std::string> FetchShowers(std::string inFile);
    
    bool isFailure;

  private:
    bool isThinned;
    bool recoveryMode;
    
    int blockSize;
    int subBlockSize;
    int numSubBlocks;

    unsigned int showerCount;
    unsigned int blocksRead;
    unsigned int blockIndex;
    unsigned int lastEVTEBlockIndex;

    
    std::string fName;
    
    bool ReadNewBlock();
    std::string GetSubBlockType();
    bool ReadDataSubBlock();
    void ReadEventHeader();
    void RecoverShowers();

    float* blockBuff;
    float* subBlockBuff;
    char* sizeBuff;
    std::string head;

    static constexpr int blockSize_unthinned=5733;
    static constexpr int blockSize_thinned=6552;
    static constexpr int subBlockSize_unthinned=273;
    static constexpr int subBlockSize_thinned=312;

    std::ifstream fin;

    CRShower shower;

};


#endif
