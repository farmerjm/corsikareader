#include "corsikareader.h"

CorsikaFile::CorsikaFile(std::string name,bool _isThinned)  {
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
  recoveryMode=1;

  std::cout << "Opening " << fName << std::endl;

  blockBuff = new float[blockSize];
  subBlockBuff = new float[subBlockSize];
  sizeBuff = new char[4];

  fin.open(fName, std::ios::in | std::ios::binary );
  if (!fin.is_open()) std::cerr << "File is not open!" << std::endl;
  if (!fin) std::cerr << "ERROR OPENING FILE! " << std::endl;
  if (!fin) std::cerr << "Error:  " << strerror(errno) << std::endl;
  
  showerCount=0;
  blocksRead=0;
  isFailure=0;
  ReadNewBlock();
}


CorsikaFile::~CorsikaFile() {
  delete[] blockBuff;
  delete[] subBlockBuff;
  delete[] sizeBuff;
}


bool CorsikaFile::ReadNewBlock() {
  //std::cout << "Reading new block..." << std::endl;
  fin.read(sizeBuff, 4);
  fin.read((char*)blockBuff, blockSize*4);
  fin.read(sizeBuff,4);
  if (fin.eof()) { 
    std::cout << "EOF. Particles read:  " << shower.GetNParticles() << std::endl;
    return 0;
  }
  blocksRead++;
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



void CorsikaFile::ReadEventHeader() {
  //Finding a new header means finding a new event, so now we initialize a new shower.
  double energy=subBlockBuff[3];
  double zenith=subBlockBuff[10];
  double azimuth=subBlockBuff[11];
  double thinLevel=subBlockBuff[148]; //EM thinning;
  double bx = subBlockBuff[70];
  double bz = subBlockBuff[71];

  shower.energy=energy;
  shower.zenith=zenith;
  shower.azimuth=azimuth;
  shower.thinLevel=thinLevel;
  shower.bx=bx;
  shower.bz=bz;
}

bool CorsikaFile::ReadDataSubBlock() {
  float desc;
  float px, py, pz;
  float x,y,z,t;

  bool wasLastAddiMuon=0; 
  

  for (int j=0; j<39; j++) {
    int ind;
    if (isThinned) ind=j*8; else ind=j*7;
    float weight=-1;

    desc = subBlockBuff[ind];
    if (desc==0) break;
    unsigned int id = ((int)desc/1000);
    if (id==75 || id ==76) {
      z=subBlockBuff[ind+6];
      wasLastAddiMuon=1;
      continue;
    }
    px = subBlockBuff[ind+1];
    py = subBlockBuff[ind+2];
    pz = subBlockBuff[ind+3];

    //x = subBlockBuff[ind+4]/100;
    //y = subBlockBuff[ind+5]/100;
    x = subBlockBuff[ind+4]/100;
    y = subBlockBuff[ind+5]/100;
    t = subBlockBuff[ind+6];

    if (isThinned) weight=subBlockBuff[ind+7];

    CorsikaParticle* part = new CorsikaParticle(); 
    part->SetDescription(desc);
    part->SetMomentum(px, py, pz);
    part->SetPosition(x, y);
    part->SetTime(t);
    part->SetWeight(weight);
    if (wasLastAddiMuon) part->SetZ(z);
    shower.AddParticle(part);
  }
  //part->Dump();
  return 1;
}

//WIP
void CorsikaFile::RecoverShowers() {
  std::cout << "Entering recovery mode..." << std::endl;
    //Copy as many blocks as were read in the blocksRead file
    //Copy as many subblocks as can into a block buffer;
    //append a RUNE and fill the rest with zeros

  fin.open(fName, std::ios::in | std::ios::binary );
  std::ofstream fout(fName+".rec", std::ios::binary);
  unsigned int blocksReadBeforeFailure=blocksRead;
  blocksRead=0;
  while(1) {
    //Copy as many blocks as were read before crashing.
    if (blocksRead < blocksReadBeforeFailure-1) {     //check endpoints here.
      ReadNewBlock();
      fout.write(sizeBuff,4);
      fout.write((char*)blockBuff, blockSize*4);
      fout.write(sizeBuff,4);
      continue;
    }

    //Read as many finished sublocks as exist in the file.
    for (int i=0; i<blockSize; i++) blockBuff[i]=0;
    fin.read(sizeBuff, 4);
    fin.read((char*)blockBuff, lastEVTEBlockIndex*subBlockSize*4);
    fin.close();

    fout.write(sizeBuff,4);
    fout.write((char*)blockBuff, numSubBlocks*subBlockSize*4);
    fout.write(sizeBuff,4);

    return;
  }
}

std::vector<std::string> CorsikaFile::FetchShowers(std::string inFile) {
  std::vector<std::string> files;
  std::ifstream fin(inFile);
  std::string line;
  while (std::getline(fin,line)) {
    size_t index = line.rfind(".", line.length());
    if (index == std::string::npos) files.push_back(line);
  }
  return files;
}

std::vector<std::string> CorsikaFile::FetchShowersFromDir(std::string inDir) {
  std::vector<std::string> files;
  DIR *dp;
  struct dirent *dirp;
  if ((dp=opendir(inDir.c_str()))==NULL) {
    std::cerr << "ERROR opening dir " << inDir << std::endl;
  }

  std::string theName;
  while ((dirp = readdir(dp))!=NULL) {
    theName=std::string(dirp->d_name);
    if (theName != "." && theName != "..") {
      size_t index = theName.rfind(".", theName.length());
      if (index==std::string::npos) continue;
      if (theName.substr(index,theName.length()) == ".inclined") files.push_back(theName);
      //if (index == std::string::npos) files.push_back(theName);
    }
  }
  closedir(dp);
  return files;
}





bool CorsikaFile::ReadNewShower() {
  shower.ClearParticles();
  while(1) {
    if (blockIndex == numSubBlocks) {
      blockIndex=0;
      if (!ReadNewBlock()) {
        fin.close();
        std::cout << "Blockread failure! " << blocksRead << " blocks read" << std::endl;
        isFailure=1;
        //if (recoveryMode) RecoverShowers();
        return 0;
      }
      //std::cout << "Reading new block!" << std::endl;
    }

    for (int j=0; j<subBlockSize; j++) {
      subBlockBuff[j]=blockBuff[blockIndex*subBlockSize+j];
    }
    blockIndex++;

    //Read subblock type and determine action
    head = GetSubBlockType();
    //if (!head.empty() && (head != "DATA")) std::cout << head << std::endl;
    if (head == "EVTH") ReadEventHeader();
    if (head == "DATA") ReadDataSubBlock();
    if (head == "EVTE") {
      showerCount++; 
      lastEVTEBlockIndex=blockIndex;
      break; 
    }
    if (head == "RUNE") {
      std::cout << "Run end reached. " << blocksRead << " blocks read" <<  std::endl;
      return 0;
    }
  }
  //std::cout << "Shower break. Particles read:  " << shower.GetNParticles() << std::endl;
  return 1;
}
