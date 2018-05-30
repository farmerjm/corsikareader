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

bool CorsikaParticle::IsMuonic() const {
  if (id == 75 || id == 76) return true;  //MUADDI
  if (id == 5 || id == 6) return true;   //THis double-counts if muaddi is on
  return false;
}

bool CorsikaParticle::IsElectromagnetic() const {
  if (id == 0) return true;  //photons
  if ((id == 1) || (id == 2)) return true; //e+/e-
  if (id ==7) return true;    //Pi_0 (-> photon)
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
