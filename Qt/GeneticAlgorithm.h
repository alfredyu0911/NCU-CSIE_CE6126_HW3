#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "AllEnumerate.h"
#include "AllStructure.h"
#include "RBFN.h"

#include <vector>

using namespace std;

class GeneticAlgorithm
{
public:
    GeneticAlgorithm(int numberOfHiddenNeural_J, vector<DataContent> vInputData);
    ~GeneticAlgorithm();

    // 外部呼叫操作.
    void updateSetting(int population, int crossoverRate, int mutationRate, int generation);
    bool GA_strat();

    // 外部取值.
    float getBestFitnessValue();
    float getBsetAverageError();
    float getAngleByInputToRBFN(float disF, float disR, float disL, float x=0, float y=0);
    Particule getBestResult();

    // 外部檢查.
    int checkProgress() { return m_nProgress/* * 100 / m_nGeneration*/; }
    int getExecuteTime() { return m_nExecuteTimeInSecond; }

protected:
    // GA 主流程.
    bool normalizeInputData();
    void initAllChromosome();
    bool calcAllFitnessValue();
    bool reproduction();
    bool crossover();
    bool mutaion();

protected:
    TrainDataType m_dataType;
    RBFN *m_RBFN_network;
    int m_nPopulation, m_nCrossoverRate, m_nMutationRate, m_nGeneration;
    int m_nDim_Chromosome;
    vector<DataContent> m_vInputData, m_vNormalizedData;
    vector<Particule> m_vParticules;

    float getFitnessValue(Particule p);

    // 正規化、反正規化處理.
    float distanceNormalize(float dis);
    float positionNormalize(float pos);
    float angleNormalize(float angle);
    float angleDenormalize(float angle);

private:
    GeneticAlgorithm();
    int m_nProgress;
    int m_nExecuteTimeInSecond;
};

#endif // GENETICALGORITHM_H
