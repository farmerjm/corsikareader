#ifndef CRPART_H
#define CRPART_H


#include <iostream>
#include <math.h>

class CorsikaParticle {


  public:
    CorsikaParticle();
    ~CorsikaParticle();

    void SetMomentum(double _px, double _py, double _pz);
    void SetPosition(double _x, double _y);
    void SetTime(double _t);
    void SetDescription(float desc);
    void SetWeight(float _weight);

    void SetZ(double _z); //for muaddi

    bool IsMuonic(); 

    float GetEnergy() {return sqrt(px*px+py*py+pz*pz);}

    void Dump();

    int id, gen, obslev;
    double x,y,z;
    double px, py, pz;
    double weight;
    double time;

};
#endif
