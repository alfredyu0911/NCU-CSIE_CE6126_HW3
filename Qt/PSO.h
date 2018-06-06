#ifndef PSO_H
#define PSO_H

#include <QObject>
#include <QTimer>

#include "AllStructure.h"

class PSO : public QObject
{
    Q_OBJECT
public:
    explicit PSO(QObject *parent = nullptr);
    ~PSO();
    void setParameter(int generation, int population, float phi_1, float phi_2, int J);
    void setTrainData(vector<DataContent> vData) { m_vAllTrainData = vData; }

    void run(int n);

    int getProgress() { return m_nProgress; }
    int getExecutionTime() { return m_nExecutionTime; }

    vector<Particule> getBestParticules() { return m_vResultParticule; }
    vector<float> getBestFitnessValues() { return m_vResultFitnessValue; }
    vector<float> getBsetAverageErrors() { return m_vResultAverageError; }

    bool isExperimentMode() { return m_nRunTimes != 1; }

protected:
    int m_nProgress, m_nGeneration, m_nPopulation, m_nJ, m_nRunTimes;
    int m_nExecutionTime, m_nMultipleStartTimes;
    float m_fPhi_1, m_fPhi_2;
    QTimer *m_timer;
    vector<Particule> m_vAllParticule;
    vector<DataContent> m_vAllTrainData, m_vNormalizedData;

    vector<Particule> m_vResultParticule;
    vector<float> m_vResultFitnessValue;
    vector<float> m_vResultAverageError;

protected:
    bool normalizeInputData();
    void initPopulation();
    void updateGlobalAndLocalBest();
    void swarmMove();

protected:
    void calcFitnessValue();
    float calcAverageErrors();
    float getFitnessValue(Particule p);

    // 正規化、反正規化處理.
    float distanceNormalize(float dis);
    float positionNormalize(float pos);
    float angleNormalize(float angle);
    float angleDenormalize(float angle);

signals:
    void PSO_Did_start();
    void PSO_Did_finsih();
    void PSO_error();

private slots:
    void PSO_Start();
};

#endif // PSO_H
