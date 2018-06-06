#include "GA_ThreadRunner.h"
#include <QTime>

GA_ThreadRunner::GA_ThreadRunner(QObject *parent) : QObject(parent)
{
    initRunner();
}

void GA_ThreadRunner::doRun()
{
    m_timer->stop();

    emit runner_didStart();
    if ( m_GA->GA_strat() )
    {
        m_vBestResult.push_back(m_GA->getBestResult());
        emit runner_didFinish();
    }
    else
    {
        emit runner_error();
    }
}

void GA_ThreadRunner::initRunner()
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->start(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(doRun()));
}

void GA_ThreadRunner::input_GA_parameter(int numberOfHiddenNeural_J, vector<DataContent> vInputData)
{
    m_GA = new GeneticAlgorithm(numberOfHiddenNeural_J, vInputData);
}

void GA_ThreadRunner::update_GA_setting(int population, int crossoverRate, int mutationRate, int generation)
{
    m_GA->updateSetting(population, crossoverRate, mutationRate, generation);
}

float GA_ThreadRunner::getAngle(float disF, float disR, float disL, float x, float y)
{
    return m_GA->getAngleByInputToRBFN(disF, disR, disL, x, y);
}

float GA_ThreadRunner::getBestFitnessValue()
{
    return m_GA->getBestFitnessValue();
}

int GA_ThreadRunner::checkProgress()
{
    return m_GA->checkProgress();
}

int GA_ThreadRunner::checkExecuteTime()
{
    return m_GA->getExecuteTime();
}

float GA_ThreadRunner::getBsetAverageError()
{
    return m_GA->getBsetAverageError();
}

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+- //

GA_ThreadMultipleRunner::GA_ThreadMultipleRunner(QObject *parent) : QObject(parent)
{
    initRunner();
}

void GA_ThreadMultipleRunner::initRunner()
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(true);
    m_timer->start(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(doRun()));
}

float GA_ThreadMultipleRunner::getAngle(float disF, float disR, float disL, float x, float y)
{
    return m_GA->getAngleByInputToRBFN(disF, disR, disL, x, y);
}

void GA_ThreadMultipleRunner::input_GA_parameter(int numberOfHiddenNeural_J, vector<DataContent> vInputData)
{
    m_GA = new GeneticAlgorithm(numberOfHiddenNeural_J, vInputData);
}

void GA_ThreadMultipleRunner::update_GA_setting(int population, int crossoverRate, int mutationRate, int generation)
{
    m_nGeneration = generation;
    m_GA->updateSetting(population, crossoverRate, mutationRate, m_nGeneration);
}

void GA_ThreadMultipleRunner::doRun()
{
    m_timer->stop();

    QTime qtime = QTime::currentTime();
    qtime.start();

    m_vBestFitnessValue.clear();
    m_vBestAverageError.clear();

    emit multipleRunner_didStart();

    for ( m_nProgress=0 ; m_nProgress < m_nExperimentTime ; m_nProgress++ )
    {
        if ( false == m_GA->GA_strat() )
        {
            m_nExecuteTime = qtime.elapsed();
            emit multipleRunner_error();
        }
        else
        {
            m_vBestFitnessValue.push_back(m_GA->getBestFitnessValue());
            m_vBestAverageError.push_back(m_GA->getBsetAverageError());
            m_vBestResult.push_back(m_GA->getBestResult());
        }
    }
    m_nExecuteTime = qtime.elapsed();
    emit multipleRunner_didFinish();
}

int GA_ThreadMultipleRunner::checkProgress()
{
    return m_GA->checkProgress() + m_nProgress*m_nGeneration;
}
