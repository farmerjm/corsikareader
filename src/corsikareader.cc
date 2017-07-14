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

  fName=name;
  numSubBlocks=21;
  first=1;

  fin.open(fName, std::ios::in | std::ios::binary );
  if (!fin.is_open()) std::cerr << "File is not open!" << std::endl;
  if (!fin) std::cerr << "ERROR OPENING FILE! " << std::endl;
  if (!fin) std::cerr << "Error:  " << strerror(errno) << std::endl;

}


CorsikaFile::~CorsikaFile() {

}


//ifstream read
void CorsikaFile::Read() {
  float* blockBuff = new float[blockSize];
  float* subBlockBuff = new float[subBlockSize];
  char* sizeBuff = new char[4];

  long c = 0;
  while(fin) {
    fin.read(sizeBuff, 4);
    fin.read((char*)blockBuff, blockSize*4);
    fin.read(sizeBuff,4);
    if (fin.eof()) break;

    int index=0;
    bool isData;
    for (int i=0; i<numSubBlocks; i++) {

      for (int j=0; j<subBlockSize; j++) {
        subBlockBuff[j]=blockBuff[index+j];
      }

      index=index+subBlockSize;
      std::string head;

      if (subBlockBuff[0] >= 211284 && subBlockBuff[0] <= 211286) head = "RUNH";
      if (subBlockBuff[0] >= 217432 && subBlockBuff[0] <= 217434) head = "EVTH";
      if (subBlockBuff[0] >= 52814  && subBlockBuff[0] <= 52816) head = "LONG";
      if (subBlockBuff[0] >= 3396   && subBlockBuff[0] <= 3398) head = "EVTE";
      if (subBlockBuff[0] >= 3300   && subBlockBuff[0] <= 3302) head = "RUNE";
      if (head.empty()) isData=1;

      if (isData) {
        for (int j=0; j<39; j++) {
          int ind;
          if (isThinned) ind=j*8; else ind=j*7;
          float desc;
          float px, py, pz;
          float x,y,z,t;
          float weight=-1;

          desc = subBlockBuff[ind];
          if (desc==0) break;
          px = subBlockBuff[ind+1];
          py = subBlockBuff[ind+2];
          pz = subBlockBuff[ind+3];

          x = subBlockBuff[ind+4]/100;
          y = subBlockBuff[ind+5]/100;
          z = subBlockBuff[ind+6]/100;

          if (isThinned) weight=subBlockBuff[ind+7];

          
          CorsikaParticle* part = new CorsikaParticle(); 
          part->SetDescription(desc);
          part->SetMomentum(px, py, pz);
          part->SetPosition(x, y, z);
          part->SetWeight(weight);
          ParticleList.push_back(part);
          
          //part->Dump();
        }
      }
    }
  }
}
  
bool CorsikaFile::ReadNewShower() {
  ParticleList.clear();
  float* blockBuff = new float[blockSize];
  float* subBlockBuff = new float[subBlockSize];
  char* sizeBuff = new char[4];

  long c = 0;
  bool doBreak=0;
  while(1) {
    fin.read(sizeBuff, 4);
    fin.read((char*)blockBuff, blockSize*4);
    fin.read(sizeBuff,4);
    if (fin.eof()) {
      std::cout << "EOF. Particles read:  " << ParticleList.size() << std::endl;
      return 0;
    }
    int index=0;
    bool isData;

    for (int i=0; i<numSubBlocks; i++) {
      for (int j=0; j<subBlockSize; j++) {
        subBlockBuff[j]=blockBuff[index+j];
      }

      index=index+subBlockSize;
      std::string head;

      if (subBlockBuff[0] >= 211284 && subBlockBuff[0] <= 211286) head = "RUNH";
      if (subBlockBuff[0] >= 217432 && subBlockBuff[0] <= 217434) head = "EVTH";
      if (subBlockBuff[0] >= 52814  && subBlockBuff[0] <= 52816) head = "LONG";
      if (subBlockBuff[0] >= 3396   && subBlockBuff[0] <= 3398) head = "EVTE";
      if (subBlockBuff[0] >= 3300   && subBlockBuff[0] <= 3302) head = "RUNE";

      if (!head.empty()) std::cout << head << std::endl;
      if (head == "EVTE") doBreak=1;

      if (head.empty()) isData=1;
      
      if (isData) {
        for (int j=0; j<39; j++) {
          int ind;
          if (isThinned) ind=j*8; else ind=j*7;
          float desc;
          float px, py, pz;
          float x,y,z,t;
          float weight=-1;

          desc = subBlockBuff[ind];
          if (desc==0) break;
          px = subBlockBuff[ind+1];
          py = subBlockBuff[ind+2];
          pz = subBlockBuff[ind+3];

          x = subBlockBuff[ind+4]/100;
          y = subBlockBuff[ind+5]/100;
          z = subBlockBuff[ind+6]/100;

          if (isThinned) weight=subBlockBuff[ind+7];

          
          CorsikaParticle* part = new CorsikaParticle(); 
          part->SetDescription(desc);
          part->SetMomentum(px, py, pz);
          part->SetPosition(x, y, z);
          part->SetWeight(weight);
          ParticleList.push_back(part);
          
          //part->Dump();
        }
      }
    }
    if (doBreak) break;
  }
  std::cout << "Shower break. Particles read:  " << ParticleList.size() << std::endl;
  return 1;
}
