#include "corsikaparticle.h"


CorsikaParticle::CorsikaParticle() {}
CorsikaParticle::~CorsikaParticle() {}

void CorsikaParticle::SetMomentum(double _px, double _py, double _pz) {
  px=_px;
  py=_py;
  pz=_pz;
}

void CorsikaParticle::SetPosition(double _x, double _y) {
 x=_x;
 y=_y;
}

void CorsikaParticle::SetTime(double _t) {
  time=_t;
}

void CorsikaParticle::SetZ(double _z) {
  z=_z;
}

bool CorsikaParticle::IsMuonic() {
  if (id == 75 || id == 76) return true;
  if (id == 5 || id == 6) return true;
  return false;
}
  

void CorsikaParticle::SetWeight(float _weight) {
  weight=_weight;
}

void CorsikaParticle::SetDescription(float desc) {
  id = ((int)desc)/1000;
  gen = (((int)desc)/10)%10;
  obslev = ((int)desc)%10;
}


void CorsikaParticle::Dump() {
  std::cout << "Momentum:  " << px << " " << py  << " " << pz << std::endl;
  std::cout << "Position: " << x << " " << y << " " << z << std::endl;
  std::cout << "id/gen/obs: " << id << " " << gen << " " << obslev << std::endl;
  std::cout << "weight: " << weight << std::endl;
}
