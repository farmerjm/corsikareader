#include "crshower.h"

unsigned int CRShower::ReserveN;

void CRShower::ClearParticles() {
  for (auto part : ParticleList) delete part;
  ParticleList.clear();
}

void CRShower::AddParticle(CorsikaParticle* part) {
  ParticleList.push_back(part);
}


CRShower::CRShower() {
  ParticleList.reserve(ReserveN);
  energy=0;
  zenith=0;
  azimuth=0;
  thinLevel=0;
}

CRShower::~CRShower() {
  //ClearParticles();
}
