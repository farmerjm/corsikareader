#include "crshower.h"
void CRShower::ClearParticles() {
  for (auto part : ParticleList) delete part;
  ParticleList.clear();
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
