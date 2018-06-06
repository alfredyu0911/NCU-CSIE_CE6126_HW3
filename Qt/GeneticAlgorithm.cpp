#include "GeneticAlgorithm.h"

#include <ctime>
#include <cmath>
#include <cstdlib>

#include <QTime>

static inline bool sort_compare(Particule a, Particule b)
{
    return a.fitness < b.fitness;
}

GeneticAlgorithm::GeneticAlgorithm(int numberOfHiddenNeural_J, vector<DataContent> vInputData)
{
    m_dataType = vInputData[0].dataType;
    m_RBFN_network = new RBFN(Particule());

    int nDim_X = (int)vInputData[0].dataType - 1;
    int nDim_J = numberOfHiddenNeural_J;

    // 1 + j + i*j + j
    m_nDim_Chromosome = 1 + nDim_J + nDim_J*nDim_X + nDim_J;

    m_vInputData = vInputData;
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    m_vParticules.clear();

    if ( m_RBFN_network != NULL )
        delete m_RBFN_network;
}

void GeneticAlgorithm::updateSetting(int population, int crossoverRate, int mutationRate, int generation)
{
    m_nPopulation = population;
    m_nCrossoverRate = crossoverRate;
    m_nMutationRate = mutationRate;
    m_nGeneration = generation;
}

bool GeneticAlgorithm::GA_strat()
{
    QTime qtime = QTime::currentTime();
    qtime.start();

    if ( normalizeInputData() == false )
    {
        m_nExecuteTimeInSecond = qtime.elapsed();
        return false;
    }

    initAllChromosome();

    for ( m_nProgress=0 ; m_nProgress < m_nGeneration ; m_nProgress++ )
    {
        bool bFail = false;

        bFail |= !calcAllFitnessValue();
        bFail |= !reproduction();
        bFail |= !crossover();
        bFail |= !mutaion();

        if ( bFail )
        {
            m_nExecuteTimeInSecond = qtime.elapsed();
            return false;
        }
    }

    m_nExecuteTimeInSecond = qtime.elapsed();
    return true;
}

float GeneticAlgorithm::getBestFitnessValue()
{
    calcAllFitnessValue();
    sort(m_vParticules.begin(), m_vParticules.end(), sort_compare);
    return getFitnessValue(m_vParticules[0]);
}

float GeneticAlgorithm::getBsetAverageError()
{
    calcAllFitnessValue();
    sort(m_vParticules.begin(), m_vParticules.end(), sort_compare);
    m_RBFN_network->resetParameterByInputParticule(m_vParticules[0]);

    int nSize = (int)m_vNormalizedData.size();
    float totalError = 0.0;

    InputVector input(m_dataType);
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

        float networkOutput = m_RBFN_network->getResultByInputVector(input);

        totalError += fabs(m_vNormalizedData[i].wheelAngle - networkOutput);
    }

    return totalError / (float)nSize;
}

float GeneticAlgorithm::getAngleByInputToRBFN(float disF, float disR, float disL, float x, float y)
{
    InputVector X = InputVector(m_dataType);
    int idx = 0;
    if ( m_dataType == TrainDataType_6D )
    {
        X.aryElement[idx++] = positionNormalize(x);
        X.aryElement[idx++] = positionNormalize(y);
    }
    X.aryElement[idx++] = distanceNormalize(disF);
    X.aryElement[idx++] = distanceNormalize(disR);
    X.aryElement[idx++] = distanceNormalize(disL);

    sort(m_vParticules.begin(), m_vParticules.end(), sort_compare);

    m_RBFN_network->resetParameterByInputParticule(m_vParticules[0]);
    float angle = m_RBFN_network->getResultByInputVector(X);

    return angleDenormalize(angle);
}

bool GeneticAlgorithm::calcAllFitnessValue()
{
    float fit_total=0.0;
    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        float fit = getFitnessValue(m_vParticules[i]);
        m_vParticules[i].fitness = fit;
        fit_total += fit;
    }

    float fit_avg = fit_total / (float)m_vParticules.size();
    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        // 適性值小者為勝.
        m_vParticules[i].toNextGeneration = round(fit_avg / m_vParticules[i].fitness);
    }

    return true;
}

bool GeneticAlgorithm::reproduction()
{
    // 先依照適應函數值排序.
    sort(m_vParticules.begin(), m_vParticules.end(), sort_compare);

    vector<Particule> vNewParticule;
    vNewParticule.clear();

    // 操作前後須保持族群大小.
    int remain = m_nPopulation;

    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        for ( int k=0 ; k < m_vParticules[i].toNextGeneration ; k++ )
        {
            if ( remain > 0 )
            {
                Particule newP;
                newP.initGeneSerialArray(m_vParticules[i].geneDimension);
                newP.copyFromInput(m_vParticules[i]);
                vNewParticule.push_back(newP);
                remain--;
            }
        }

        // 因為輪盤式篩選，比例計算時的 round，四捨五入最後有可能導致會缺，所以這邊要補回去.
        if ( remain > 0 && m_vParticules[i].toNextGeneration == 0 )
        {
            Particule newP;
            newP.initGeneSerialArray(m_vParticules[i].geneDimension);
            newP.copyFromInput(m_vParticules[i]);
            vNewParticule.push_back(newP);
            remain--;
        }
    }

    m_vParticules.clear();
    m_vParticules = vNewParticule;

    return m_nPopulation == (int)m_vParticules.size();
}
/*
bool GeneticAlgorithm::crossover()
{
    // 基因序列長度皆相同，這邊採隨機遮罩交換，遮罩長度為隱藏神經元個數 J、且跳過第零個染色體.
    // 隨機取位置 (dim = 1 + j + i*j + j)、跳過第零個染色體，隨機換"一組"
    int pos = (rand() % ((m_nDim_Chromosome-1) / (m_nDim_x+2))) + 1;

    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        int random = rand() % 100;

        // 對每一個個體而言，有百分之 m_nCrossoverRate 的機率發生交配.
        if ( random <= m_nCrossoverRate )
        {
            // 隨機取另一個"不同的"個體.
            int j=i;
            while ( j == i )
                j = rand() % m_nPopulation;

            // 遮罩交換.
            for ( int idx=0 ; idx < m_nDim_J ; idx++ )
            {
                float temp = m_vParticules[i].aryGeneSerial[pos+idx];
                m_vParticules[i].aryGeneSerial[pos+idx] = m_vParticules[j].aryGeneSerial[pos+idx];
                m_vParticules[j].aryGeneSerial[pos+idx] = temp;
            }
        }
    }

    return true;
}*/

bool GeneticAlgorithm::crossover()
{
    // 基因序列長度皆相同，這邊採隨機遮罩交換、隨機遮罩長度(最多一半)
    int pos = rand() % (m_nDim_Chromosome/2);
    int len = rand() % (m_nDim_Chromosome/2) + 1;

    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        // 對每一個個體而言，有百分之 m_nCrossoverRate 的機率發生交配.
        int random = rand() % 100;
        if ( random <= m_nCrossoverRate )
        {
            // 隨機取另一個"不同的"個體.
            int j=i;
            while ( j == i )
                j = rand() % m_nPopulation;

            // 遮罩交換.
            for ( int idx=0 ; idx < len ; idx++ )
            {
                float temp = m_vParticules[i].aryGeneSerial[pos+idx];
                m_vParticules[i].aryGeneSerial[pos+idx] = m_vParticules[j].aryGeneSerial[pos+idx];
                m_vParticules[j].aryGeneSerial[pos+idx] = temp;
            }
        }
    }

    return true;
}

bool GeneticAlgorithm::mutaion()
{
    for ( int i=0 ; i < (int)m_vParticules.size() ; i++ )
    {
        // 對每一個個體而言，有百分之 m_nMutationRate 的機率發生變異.
        // 且每個個體皆有多重變異的可能.
        while ( (rand() % 100) <= m_nMutationRate )
        {
            // 隨機取一位置.
            int pos = rand() % m_nDim_Chromosome;

            // 隨機取變異量，範圍 +-0.3
            float delta = rand()*0.6 / RAND_MAX + (-0.3);

            // 變異.
            m_vParticules[i].aryGeneSerial[pos] += delta;

            // 飽和運算.
            (m_vParticules[i].aryGeneSerial[pos] >  1.0) ?  1.0 : m_vParticules[i].aryGeneSerial[pos];
            (m_vParticules[i].aryGeneSerial[pos] < -1.0) ? -1.0 : m_vParticules[i].aryGeneSerial[pos];
        }
    }

    return true;
}

bool GeneticAlgorithm::normalizeInputData()
{
    TrainDataType type = m_vInputData[0].dataType;
    m_vNormalizedData.clear();

    for ( int i=0 ; i < (int)m_vInputData.size() ; i++ )
    {
        if ( type != m_vInputData[i].dataType )
            return false;

        DataContent data;
        data.dataType = type;

        // Normalize
        data.distanceFront = distanceNormalize(m_vInputData[i].distanceFront);
        data.distanceRight = distanceNormalize(m_vInputData[i].distanceRight);
        data.distanceLeft  = distanceNormalize(m_vInputData[i].distanceLeft);
        data.wheelAngle = angleNormalize(m_vInputData[i].wheelAngle);
        if ( type == TrainDataType_6D )
        {
            data.positionX = positionNormalize(m_vInputData[i].positionX);
            data.positionY = positionNormalize(m_vInputData[i].positionY);
        }

        m_vNormalizedData.push_back(data);
    }

    return true;
}

void GeneticAlgorithm::initAllChromosome()
{
    m_vParticules.clear();

    for ( int i=0 ; i < m_nPopulation ; i++ )
    {
        Particule p;
        p.initGeneSerialArray(m_nDim_Chromosome);
        p.randomlyAssignGeneSerialArray();
        m_vParticules.push_back(p);
//        p.release();
    }
}

float GeneticAlgorithm::getFitnessValue(Particule p)
{
    float fit = 0.0;
    InputVector input(m_dataType);

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

        m_RBFN_network->resetParameterByInputParticule(p);
        float networkOutput = m_RBFN_network->getResultByInputVector(input);

        fit += pow(m_vNormalizedData[i].wheelAngle - networkOutput, 2.0);
    }

    return fit / 2.0;
}

float GeneticAlgorithm::distanceNormalize(float dis)
{
    // 0~80  ->  -1~1
    return (dis - 40.0) / 40.0;
}

float GeneticAlgorithm::positionNormalize(float pos)
{
    // -10~70  ->  -1~1
    return (pos - 30.0) / 40.0;
}

float GeneticAlgorithm::angleNormalize(float angle)
{
    // -40~40  ->  -1~1
    return angle / 40.0;
}

float GeneticAlgorithm::angleDenormalize(float angle)
{
    // -1~1  ->  -40~40
    return angle * 40.0;
}

Particule GeneticAlgorithm::getBestResult()
{
    sort(m_vParticules.begin(), m_vParticules.end(), sort_compare);
    return m_vParticules[0];
}
