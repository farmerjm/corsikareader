#include "crshower.h"
void CRShower::ClearParticles() {
  for (auto part : ParticleList) delete part;
  ParticleList.clear();
}

void CRShower::AddParticle(CorsikaParticle* part) {
  ParticleList.push_back(part);
  if (separateComponents) {
    if (part->IsMuonic()) MuonicComponent.push_back(part); else EMComponent.push_back(part);
  }
}


const std::vector<CorsikaParticle*>& CRShower::GetMuonicParticles() {
  if (separateComponents) return MuonicComponent;
  return std::vector<CorsikaParticle*>(0,0);
}

const std::vector<CorsikaParticle*>& CRShower::GetEMParticles() {
  if (separateComponents) return EMComponent;
  return std::vector<CorsikaParticle*>(0,0);  
}


CRShower::CRShower() {
  energy=0;
  zenith=0;
  azimuth=0;
  thinLevel=0;
}

CRShower::~CRShower() {
  //ClearParticles();
}
