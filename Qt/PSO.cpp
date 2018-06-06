#include "PSO.h"
#include "RBFN.h"

#include <QTime>

static inline bool sort_compare(Particule a, Particule b)
{
    return a.fitness < b.fitness;
}

PSO::PSO(QObject *parent) : QObject(parent)
{
    m_nProgress = 0;
    m_timer = new QTimer();
    m_vAllParticule.clear();
}

PSO::~PSO()
{
    if ( m_timer != NULL )
    {
        m_timer->stop();
        delete m_timer;
    }
}

void PSO::setParameter(int generation, int population, float phi_1, float phi_2, int J)
{
    m_nGeneration = generation;
    m_nPopulation = population;
    m_fPhi_1 = phi_1 / 1000.0;
    m_fPhi_2 = phi_2 / 1000.0;
    m_nJ = J;
}

void PSO::run(int n)
{
    m_nRunTimes = n;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(PSO_Start()));
    m_timer->setSingleShot(true);
    m_timer->start(50);
}

void PSO::PSO_Start()
{
    QTime qtime = QTime::currentTime();
    qtime.start();
    emit PSO_Did_start();

    if ( normalizeInputData() == false )
    {
        m_nExecutionTime = qtime.elapsed();
        emit PSO_error();
    }

    m_nProgress = 0;
    m_vResultParticule.clear();
    m_vResultFitnessValue.clear();
    m_vResultAverageError.clear();
    for ( int i=0 ; i < m_nRunTimes ; i++ )
    {
        initPopulation();

        for ( int j=0 ; j < m_nGeneration ; j++ )
        {
            calcFitnessValue();
            updateGlobalAndLocalBest();
            swarmMove();
            m_nProgress++;
        }

        sort(m_vAllParticule.begin(), m_vAllParticule.end(), sort_compare);
        m_vResultParticule.push_back(m_vAllParticule[0]);
        m_vResultFitnessValue.push_back(m_vAllParticule[0].fitness);
        m_vResultAverageError.push_back(calcAverageErrors());
    }

    m_nExecutionTime = qtime.elapsed();
    emit PSO_Did_finsih();
}

void PSO::initPopulation()
{
    m_vAllParticule.clear();
    int nDim = 1 + m_nJ + ((int)m_vAllTrainData[0].dataType - 1) * m_nJ + m_nJ;
    for ( int i=0 ; i < m_nPopulation ; i++ )
    {
        Particule p;
        p.initGeneSerialArray(nDim);
        p.randomlyAssignSerialAndVelocity();
        p.checkVelocity();
        m_vAllParticule.push_back(p);
    }
}

void PSO::updateGlobalAndLocalBest()
{
    sort(m_vAllParticule.begin(), m_vAllParticule.end(), sort_compare);

    for ( int i=0 ; i < (int)m_vAllParticule.size() ; i++ )
    {
        if ( m_vAllParticule[i].fitness_GB > m_vAllParticule[0].fitness )
        {
            m_vAllParticule[i].updateGlobalBest(m_vAllParticule[0]);
        }

        if ( m_vAllParticule[i].fitness_LB > m_vAllParticule[i].fitness )
        {
            m_vAllParticule[i].updateLocalBest(m_vAllParticule[i]);
        }
    }
}

void PSO::swarmMove()
{
    int nDim = 1 + m_nJ + ((int)m_vAllTrainData[0].dataType - 1) * m_nJ + m_nJ;

    for ( int i=0 ; i < (int)m_vAllParticule.size() ; i++ )
    {
        for ( int idx=0 ; idx < nDim ; idx++ )
        {
            m_vAllParticule[i].velocity[idx] += m_fPhi_1 * (m_vAllParticule[i].localBest[idx]  - m_vAllParticule[i].position[idx]) + \
                                                m_fPhi_2 * (m_vAllParticule[i].globalBest[idx] - m_vAllParticule[i].position[idx]);
            m_vAllParticule[i].checkVelocity();

            m_vAllParticule[i].position[idx] += m_vAllParticule[i].velocity[idx];
        }
    }
}

void PSO::calcFitnessValue()
{
    float fit_total=0.0;
    for ( int i=0 ; i < (int)m_vAllParticule.size() ; i++ )
    {
        float fit = getFitnessValue(m_vAllParticule[i]);
        m_vAllParticule[i].fitness = fit;
        fit_total += fit;
    }
}

float PSO::getFitnessValue(Particule p)
{
    float fit = 0.0;
    InputVector input(m_vAllTrainData[0].dataType);
    RBFN *network = new RBFN(m_vAllParticule[0]);

    for ( int i=0 ; i < (int)m_vNormalizedData.size() ; i++ )
    {
        int idx=0;
        if ( m_vNormalizedData[i].dataType == TrainDataType_6D )
        {
            input.aryElement[idx++] = m_vNormalizedData[i].positionX;
            input.aryElement[idx++] = m_vNormalizedData[i].positionY;
        }
        input.aryElement[idx++] = m_vNormalizedData[i].distanceFront;
        input.aryElement[idx++] = m_vNormalizedData[i].distanceRight;
        input.aryElement[idx++] = m_vNormalizedData[i].distanceLeft;

        float networkOutput = network->getResult(input, p);

        fit += pow(m_vNormalizedData[i].wheelAngle - networkOutput, 2.0);
    }

    delete network;
    return fit / 2.0;
}

bool PSO::normalizeInputData()
{
    TrainDataType type = m_vAllTrainData[0].dataType;
    m_vNormalizedData.clear();

    for ( int i=0 ; i < (int)m_vAllTrainData.size() ; i++ )
    {
        if ( type != m_vAllTrainData[i].dataType )
            return false;

        DataContent data;
        data.dataType = type;

        // Normalize
        data.distanceFront = distanceNormalize(m_vAllTrainData[i].distanceFront);
        data.distanceRight = distanceNormalize(m_vAllTrainData[i].distanceRight);
        data.distanceLeft  = distanceNormalize(m_vAllTrainData[i].distanceLeft);
        data.wheelAngle = angleNormalize(m_vAllTrainData[i].wheelAngle);
        if ( type == TrainDataType_6D )
        {
            data.positionX = positionNormalize(m_vAllTrainData[i].positionX);
            data.positionY = positionNormalize(m_vAllTrainData[i].positionY);
        }

        m_vNormalizedData.push_back(data);
    }

    return true;
}

float PSO::distanceNormalize(float dis)
{
    // 0~80  ->  -1~1
    return (dis - 40.0) / 40.0;
}

float PSO::positionNormalize(float pos)
{
    // -10~70  ->  -1~1
    return (pos - 30.0) / 40.0;
}

float PSO::angleNormalize(float angle)
{
    // -40~40  ->  -1~1
    return angle / 40.0;
}

float PSO::angleDenormalize(float angle)
{
    // -1~1  ->  -40~40
    return angle * 40.0;
}

float PSO::calcAverageErrors()
{
    calcFitnessValue();
    sort(m_vAllParticule.begin(), m_vAllParticule.end(), sort_compare);

    RBFN *network = new RBFN(m_vAllParticule[0]);
    network->resetParameterByInputParticule(m_vAllParticule[0]);

    int nSize = (int)m_vNormalizedData.size();
    float totalError = 0.0;

    InputVector input(m_vAllTrainData[0].dataType);
    for ( int i=0 ; i < nSize ; i++ )
    {
        int idx=0;
        if ( m_vNormalizedData[i].dataType == TrainDataType_6D )
        {
            input.aryElement[idx++] = m_vNormalizedData[i].positionX;
            input.aryElement[idx++] = m_vNormalizedData[i].positionY;
        }
        input.aryElement[idx++] = m_vNormalizedData[i].distanceFront;
        input.aryElement[idx++] = m_vNormalizedData[i].distanceRight;
        input.aryElement[idx++] = m_vNormalizedData[i].distanceLeft;

        float networkOutput = network->getResult(input, m_vAllParticule[0]);

        totalError += fabs(m_vNormalizedData[i].wheelAngle - networkOutput);
    }

    return totalError / (float)nSize;
}
