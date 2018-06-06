#include "RBFN.h"
#include <cmath>

// 基因序列
// theta, w1, w2, ..., wj, m11, m12, ..., m1i, m21, m22, ..., m2i, ..., mji, sigma1, sigma2, ..., sigmaj
//
float RBFN::getResult(const InputVector &X, const Particule &networkSetting)
{
    float result = 0.0;
    int dim_X = X.nDimension;
    int dim_J = (networkSetting.geneDimension - 1)/(dim_X + 2);  // dim = 1 + j +i*j + j   ->   j = (dim-1)/(i+2)

    for ( int j=0 ; j < dim_J ; j++ )
    {
        float squareOfVectorLength=0.0;
        for ( int i=0 ; i < dim_X ; i++ )
        {
            float Mji = networkSetting.position[1+dim_J+i+i*j];
            float Xi = X.aryElement[i];
            squareOfVectorLength += pow(Xi - Mji, 2.0);
        }
        float littelSigma = networkSetting.position[1+dim_J+dim_J*dim_X+j];
        float squareOfLittleSigma = pow(littelSigma, 2.0);
        float littelPhi = exp((-1.0)*squareOfVectorLength/2.0/squareOfLittleSigma);
        float W = networkSetting.position[1+j];

        result += W * littelPhi;
    }
    result += networkSetting.position[0];  // theta

    return result;
}
