#ifndef CRSHOW_H
#define CRSHOW_H
#include <vector>
#include "corsikaparticle.h"

//Shower container used by corsikareader.  
struct CRShower {
  private:
    std::vector<CorsikaParticle*> ParticleList;

    
  public:
    void AddParticle(CorsikaParticle* part);
    void ClearParticles();


    double energy;
    double zenith, azimuth;
    double thinLevel;

    double bx, bz;

    const bool IsThinned() const {if (thinLevel != 0) return 1; else return 0;}
    const std::vector<CorsikaParticle*>& GetParticleList() const {return ParticleList;}
    const int GetNParticles() const {return ParticleList.size();}

    CRShower();
    ~CRShower();

    static unsigned int ReserveN;

};

#endif
