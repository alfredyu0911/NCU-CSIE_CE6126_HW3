#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QSlider>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTimer>
#include <QProgressBar>
#include <QThread>

#include "CaseDataLoader.h"
#include "SystemDataCenter.h"
#include "Canvas.h"
#include "CarObject.hpp"
#include "PSO.h"
#include "RBFN.h"

using namespace std;

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void allCompoentInit();
    void addCompoentToContainer();
    void compoentEventInit();
    void setGridLayout();

    void dataInit();
    void allCompoentRefresh();

    void consoleInit();
    void consoleAddMsg(QString strMsg);
    bool isCarReachEndZone();

    QString convertTime(const int microSecond);

    void carStatusRefresh(float &disFront, float &disRight, float &disLeft);
    float getRBFN_networkOutput(RBFN *network, Particule networksetting, TrainDataType dataType, float F, float R, float L);
    void carMoveOnce(CarObject *car, float angle, bool bMessageShow, bool bAddRecord, const float F, const float R, const float L);
    bool experimentCheckIsCarSuccessfullyPassTrack(int i);

    void releaseThread();
    void setCurrentlyRunning();
    void setCurrentlyReady();

private slots:
    // view event
    void onBtnClick_CaseData();
    void onBtnClick_TrainData();
    void onBtnClick_ConsoleClear();
    void onSliderValueChange();
    void onBtnClick_PSO_Start();
    void onBtnClick_CleanAndRestart();
    void onBtnClick_PSO_MultipleStart();

private slots:
    // other event
    void onTimeOut();
    void onPSO_Start();
    void onPSO_finish();
    void onPSO_error();
    void onPSO_MultipleStart();
    void onPSO_Multiplefinish();
    void onPSO_Multipleerror();

private:
    // other object
    CaseDataLoader *m_init;
    SystemDataCenter *m_data;
    QThread *m_runThread;
    PSO *m_PSO;
    QTimer *m_timer;

// view object
private:
    vector<QWidget*> m_vAllWidget;
    QGridLayout *m_layout;
    QHBoxLayout *m_subLayout1;

    QPushButton *m_btn_CaseData, *m_btn_TrainData, *m_btn_ConsoleClear, *m_btn_clean;
    QLabel *m_lb_Generation, *m_lb_Population, *m_lbValue_Generation, *m_lbValue_Population;
    QLabel *m_lb_PSO1, *m_lb_PSO2, *m_lbValue_PSO1, *m_lbValue_PSO2, *m_lb_J, *m_lbValue_J, *m_lb_exp, *m_lbValue_exp;
    QSlider *m_slider_Generation, *m_slider_Population, *m_slider_PSO1, *m_slider_PSO2, *m_slider_J, *m_slider_exp;

    QPushButton *m_btn_StartOnce, *m_btn_StartMultiple;
    QProgressBar *m_progress_once, *m_progress_multiple;

    QPlainTextEdit *m_txtConsole;

    Canvas *m_mainCanvasView;
};

#endif // MAINWINDOW_H
