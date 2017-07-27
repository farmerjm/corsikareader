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
  blockIndex=0;

  blockBuff = new float[blockSize];
  subBlockBuff = new float[subBlockSize];
  sizeBuff = new char[4];

  fin.open(fName, std::ios::in | std::ios::binary );
  if (!fin.is_open()) std::cerr << "File is not open!" << std::endl;
  if (!fin) std::cerr << "ERROR OPENING FILE! " << std::endl;
  if (!fin) std::cerr << "Error:  " << strerror(errno) << std::endl;

  ReadNewBlock();
}


CorsikaFile::~CorsikaFile() {
  delete blockBuff;
  delete subBlockBuff;
  delete sizeBuff;
}


bool CorsikaFile::ReadNewBlock() {
  //std::cout << "Reading new block..." << std::endl;
  fin.read(sizeBuff, 4);
  fin.read((char*)blockBuff, blockSize*4);
  fin.read(sizeBuff,4);
  if (fin.eof()) {
    std::cout << "EOF. Particles read:  " << ParticleList.size() << std::endl;
    return 0;
  }
  return 1;
}

std::string CorsikaFile::GetSubBlockType() {

  if (subBlockBuff[0] >= 211284 && subBlockBuff[0] <= 211286) return "RUNH";
  if (subBlockBuff[0] >= 217432 && subBlockBuff[0] <= 217434) return "EVTH";
  if (subBlockBuff[0] >= 52814  && subBlockBuff[0] <= 52816) return "LONG";
  if (subBlockBuff[0] >= 3396   && subBlockBuff[0] <= 3398) return "EVTE";
  if (subBlockBuff[0] >= 3300   && subBlockBuff[0] <= 3302) return "RUNE";

  return "DATA";
}

bool CorsikaFile::ReadDataSubBlock() {

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
  }
  //part->Dump();
  return 1;
}

bool CorsikaFile::ReadNewShower() {
  ParticleList.clear();

  while(1) {
    if (blockIndex == numSubBlocks) {
      blockIndex=0;
      if (!ReadNewBlock()) {
        std::cout << "Blockread failure." << std::endl;
        return 0;
      }
    }

    for (int j=0; j<subBlockSize; j++) {
      subBlockBuff[j]=blockBuff[blockIndex*subBlockSize+j];
    }
    blockIndex++;

    //Read subblock type and determine action
    head = GetSubBlockType();
    if (!head.empty() && (head != "DATA")) std::cout << head << std::endl;
    if (head == "DATA") ReadDataSubBlock();
    if (head == "EVTE") break;
  }
  std::cout << "Shower break. Particles read:  " << ParticleList.size() << std::endl;
  return 1;
}
