#ifndef THREADRUNNER_H
#define THREADRUNNER_H

#include <QObject>
#include <QTimer>

#include "GeneticAlgorithm.h"
#include "AllStructure.h"
#include "AllEnumerate.h"

class GA_ThreadRunner : public QObject
{
    Q_OBJECT

public:
    explicit GA_ThreadRunner(QObject *parent = nullptr);

    void input_GA_parameter(int numberOfHiddenNeural_J, vector<DataContent> vInputData);
    void update_GA_setting(int population, int crossoverRate, int mutationRate, int generation);

    float getAngle(float disF, float disR, float disL, float x=0, float y=0);
    float getBestFitnessValue();
    float getBsetAverageError();
    vector<Particule> getAllBestResults()  { return m_vBestResult; }

    int checkProgress();
    int checkExecuteTime();

protected:
    GeneticAlgorithm *m_GA;
    QTimer *m_timer;
    vector<Particule> m_vBestResult;

    void initRunner();

signals:
    void runner_didStart();
    void runner_didFinish();
    void runner_error();

private slots:
    void doRun();
};

class GA_ThreadMultipleRunner : public QObject
{
    Q_OBJECT

public:
    explicit GA_ThreadMultipleRunner(QObject *parent = nullptr);

    void input_GA_parameter(int numberOfHiddenNeural_J, vector<DataContent> vInputData);
    void update_GA_setting(int population, int crossoverRate, int mutationRate, int generation);

    float getAngle(float disF, float disR, float disL, float x=0, float y=0);
    vector<float> getAllBestFitnessValue() { return m_vBestFitnessValue; }
    vector<float> getAllBestAverageError() { return m_vBestAverageError; }
    vector<Particule> getAllBestResults()  { return m_vBestResult; }

    void setExperimentTime(int n) { m_nExperimentTime = n; }

    int checkProgress();
    int checkExecuteTime() { return m_nExecuteTime; }

protected:
    GeneticAlgorithm *m_GA;
    QTimer *m_timer;
    vector<float> m_vBestFitnessValue;
    vector<float> m_vBestAverageError;
    vector<Particule> m_vBestResult;
    int m_nProgress, m_nGeneration, m_nExecuteTime, m_nExperimentTime;

    void initRunner();

signals:
    void multipleRunner_didStart();
    void multipleRunner_didFinish();
    void multipleRunner_error();

private slots:
    void doRun();
};

#endif // THREADRUNNER_H
