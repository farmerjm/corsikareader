#include "corsikaparticle.h"


CorsikaParticle::CorsikaParticle() {}
CorsikaParticle::~CorsikaParticle() {}

void CorsikaParticle::SetMomentum(double _px, double _py, double _pz) {
  px=_px;
  py=_py;
  pz=_pz;
}

void CorsikaParticle::SetPosition(double _x, double _y, double _z) {
 x=_x;
 y=_y;
 z=_z;
}
  

void SetWeight(float _weight) {
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
  std::cout << "weight: " << weight << endl;
}
