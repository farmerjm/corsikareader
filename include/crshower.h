#ifndef CRSHOW_H
#define CRSHOW_H
#include <vector>
#include "corsikaparticle.h"

//Shower container used by corsikareader.  
struct CRShower {
  private:
    std::vector<CorsikaParticle*> ParticleList;
    std::vector<CorsikaParticle*> MuonicComponent;
    std::vector<CorsikaParticle*> EMComponent;

    std::vector<CorsikaParticle*> BuildSplitList(bool isMuonic);
    
  public:
    void AddParticle(CorsikaParticle* part);
    void ClearParticles();

    static constexpr bool separateComponents=1;

    double energy;
    double zenith, azimuth;
    double thinLevel;

    const bool IsThinned() const {if (thinLevel != 0) return 1; else return 0;}
    const std::vector<CorsikaParticle*>& GetParticleList() {return ParticleList;}
    const std::vector<CorsikaParticle*>& GetMuonicParticles(); 
    const std::vector<CorsikaParticle*>& GetEMParticles();
    const int GetNParticles() const {return ParticleList.size();}

    CRShower();
    ~CRShower();

};

#endif
