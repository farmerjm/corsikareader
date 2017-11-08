#ifndef CRPART_H
#define CRPART_H


#include <iostream>

class CorsikaParticle {


  public:
    CorsikaParticle();
    ~CorsikaParticle();

    void SetMomentum(double _px, double _py, double _pz);
    void SetPosition(double _x, double _y, double _z);
    void SetDescription(float desc);
    void SetWeight(float _weight);

    bool IsMuonic(); 

    void Dump();

    int id, gen, obslev;
    double x,y,z;
    double px, py, pz;
    double weight;

};
#endif
