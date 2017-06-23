#include "corsikareader.h"

CorsikaFile::CorsikaFile(std::string name,bool _isThinned) {
  isThinned=_isThinned;
  
  if (isThinned) {
    blockSize=blockSize_thinned;
    subBlockSize=subBlockSize_thinned;
  } else {
    blockSize=blockSize_unthinned;
    subBlockSize=subBlockSize_unthinned;
  }

  fName=(char*)name.c_str();
  numSubBlocks=21;

}


CorsikaFile::~CorsikaFile() {

}


//ifstream read
std::vector<CorsikaParticle*> CorsikaFile::Read() {
  int numPart=0;
  float* blockBuff = new float[blockSize];
  float* subBlockBuff = new float[subBlockSize];
  char* sizeBuff = new char[4];

  std::ifstream fin(fName, std::ios::in | std::ios::binary );

  while(1) {
    fin.read(sizeBuff, 4);
    fin.read((char*)blockBuff, blockSize*4);
    fin.read(sizeBuff,4);
    if (fin.eof()) break;

    //std::cout << "Reading block:  " << blockSize  << std::endl;

    //for (int i=0; i<blockSize; i++) std::cout << blockBuff[i];

    int index=0;
    bool isData;
    for (int i=0; i<numSubBlocks; i++) {

      for (int j=0; j<subBlockSize; j++) {
        subBlockBuff[j]=blockBuff[index+j];
        index++;
      }

      std::string head;

      if (subBlockBuff[0] >= 211284 && subBlockBuff[0] <= 211286) head = "RUNH";
      if (subBlockBuff[0] >= 217432 && subBlockBuff[0] <= 217434) head = "EVTH";
      if (subBlockBuff[0] >= 52814  && subBlockBuff[0] <= 52816) head = "LONG";
      if (subBlockBuff[0] >= 3396   && subBlockBuff[0] <= 3398) head = "EVTE";
      if (subBlockBuff[0] >= 3300   && subBlockBuff[0] <= 3302) head = "RUNE";

      if (head.empty()) isData=1;
      
      if (isData) {
        for (int j=0; j<39; j++) {
          int ind=j*7;
          float desc;
          float px, py, pz;
          float x,y,z,t;

          desc = subBlockBuff[ind];
          if (desc==0) break;
          px = subBlockBuff[ind+1];
          py = subBlockBuff[ind+2];
          pz = subBlockBuff[ind+3];

          x = subBlockBuff[ind+4];
          y = subBlockBuff[ind+5];
          z = subBlockBuff[ind+6];
          
          CorsikaParticle* part = new CorsikaParticle(); 
          part->SetDescription(desc);
          part->SetMomentum(px, py, pz);
          part->SetPosition(x, y, z);
          //part->Dump();
          numPart++;
        }
      }
      //std::cout << index << " " <<  subBlockBuff[0] << " " << head << std::endl;
    }
  }

  std::cout << "numPart: " << numPart << std::endl;
}
  
