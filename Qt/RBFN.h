#ifndef RBFN_H
#define RBFN_H

#include "AllStructure.h"

class RBFN
{
public:
    RBFN(Particule p) { m_particule = p; }

    void resetParameterByInputParticule(Particule p) { m_particule = p; }
//    float getResultByInputVector(const InputVector &X);
    float getResult(const InputVector &X, const Particule &networkSetting);

private:
    Particule m_particule;
};

#endif // RBFN_H
