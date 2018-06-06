#include <QFileDialog>
#include <QScrollBar>
#include <QTime>

#include "MainWindow.h"
#include "TrainDataLoader.h"

MainWindow::MainWindow(QWidget *parent):QWidget(parent)
{
    m_layout = new QGridLayout();
    m_subLayout1 = new QHBoxLayout();
    m_data = new SystemDataCenter();
    m_timer = new QTimer();

    m_init = NULL;
    m_PSO = NULL;
    m_runThread = NULL;

    srand(time(NULL));

    allCompoentInit();
    addCompoentToContainer();
    compoentEventInit();

    setGridLayout();
}

MainWindow::~MainWindow()
{
    while ( m_vAllWidget.size() >= 1 )
    {
        QWidget *obj = m_vAllWidget.back();
        m_vAllWidget.pop_back();
        if ( obj != NULL )
            delete obj;
    }

    if ( m_subLayout1 != NULL )
        delete m_subLayout1;

    if ( m_layout != NULL )
        delete m_layout;

    if ( m_init != NULL )
        delete m_init;

    if ( m_data != NULL )
        delete m_data;

    if ( m_timer != NULL )
    {
        m_timer->stop();
        delete m_timer;
    }

    releaseThread();
}

void MainWindow::allCompoentInit()
{
    m_btn_CaseData = new QPushButton(QString("載入車軌設定"));
    m_btn_TrainData = new QPushButton(QString("載入訓練資料"));
    m_btn_ConsoleClear = new QPushButton(QString("Clear Console"));
    m_btn_clean = new QPushButton(QString("清除重來"));
    m_btn_clean->setEnabled(false);

    m_lb_Generation = new QLabel(QString("迭代次數"));
    m_slider_Generation = new QSlider(Qt::Horizontal);
    m_slider_Generation->setRange(200, 1000);
    m_slider_Generation->setValue(200);
    m_lbValue_Generation = new QLabel(QString::number(m_slider_Generation->value()));

    m_lb_Population = new QLabel(QString("族群數量"));
    m_slider_Population = new QSlider(Qt::Horizontal);
    m_slider_Population->setRange(100, 500);
    m_slider_Population->setValue(100);
    m_lbValue_Population = new QLabel(QString::number(m_slider_Population->value()));

    m_lb_PSO1 = new QLabel(QString("\u03C6-1"));
    m_slider_PSO1 = new QSlider(Qt::Horizontal);
    m_slider_PSO1->setRange(0, 1000);
    m_slider_PSO1->setValue(500);
    m_lbValue_PSO1 = new QLabel(QString("50.0%"));

    m_lb_PSO2 = new QLabel(QString("\u03C6-2"));
    m_slider_PSO2 = new QSlider(Qt::Horizontal);
    m_slider_PSO2->setRange(0, 1000);
    m_slider_PSO2->setValue(500);
    m_lbValue_PSO2 = new QLabel(QString("50.0%"));

    m_lb_J = new QLabel(QString("J"));
    m_slider_J = new QSlider(Qt::Horizontal);
    m_slider_J->setRange(1, 15);
    m_slider_J->setValue(1);
    m_lbValue_J = new QLabel(QString::number(m_slider_J->value()));

    m_lb_exp = new QLabel(QString("實驗次數"));
    m_slider_exp = new QSlider(Qt::Horizontal);
    m_slider_exp->setRange(10, 100);
    m_slider_exp->setValue(10);
    m_lbValue_exp = new QLabel(QString::number(m_slider_exp->value()));

    m_btn_StartOnce = new QPushButton(QString("start 1"));
    m_btn_StartOnce->setEnabled(false);
    m_progress_once = new QProgressBar();
    m_progress_once->setRange(0, m_slider_Generation->value());
    m_progress_once->setValue(0);

    m_btn_StartMultiple = new QPushButton(QString("start N"));
    m_btn_StartMultiple->setEnabled(false);
    m_progress_multiple = new QProgressBar();
    m_progress_multiple->setRange(0, 4748364);
    m_progress_multiple->setValue(0);

    m_txtConsole = new QPlainTextEdit();
    consoleInit();

    m_mainCanvasView = new Canvas();
}

void MainWindow::addCompoentToContainer()
{
    m_vAllWidget.push_back(m_btn_CaseData);
    m_vAllWidget.push_back(m_btn_TrainData);
    m_vAllWidget.push_back(m_btn_ConsoleClear);
    m_vAllWidget.push_back(m_btn_clean);

    m_vAllWidget.push_back(m_lb_Generation);
    m_vAllWidget.push_back(m_slider_Generation);
    m_vAllWidget.push_back(m_lbValue_Generation);

    m_vAllWidget.push_back(m_lb_Population);
    m_vAllWidget.push_back(m_slider_Population);
    m_vAllWidget.push_back(m_lbValue_Population);

    m_vAllWidget.push_back(m_lb_PSO1);
    m_vAllWidget.push_back(m_slider_PSO1);
    m_vAllWidget.push_back(m_lbValue_PSO1);

    m_vAllWidget.push_back(m_lb_PSO2);
    m_vAllWidget.push_back(m_slider_PSO2);
    m_vAllWidget.push_back(m_lbValue_PSO2);

    m_vAllWidget.push_back(m_lb_J);
    m_vAllWidget.push_back(m_slider_J);
    m_vAllWidget.push_back(m_lbValue_J);

    m_vAllWidget.push_back(m_lb_exp);
    m_vAllWidget.push_back(m_slider_exp);
    m_vAllWidget.push_back(m_lbValue_exp);

    m_vAllWidget.push_back(m_btn_StartOnce);
    m_vAllWidget.push_back(m_progress_once);

    m_vAllWidget.push_back(m_btn_StartMultiple);
    m_vAllWidget.push_back(m_progress_multiple);

    m_vAllWidget.push_back(m_txtConsole);
    m_vAllWidget.push_back(m_mainCanvasView);
}

void MainWindow::setGridLayout()
{
    // row 0
    m_subLayout1->addWidget(m_btn_CaseData);
    m_subLayout1->addWidget(m_btn_TrainData);
    m_subLayout1->addWidget(m_btn_ConsoleClear);
    m_subLayout1->addWidget(m_btn_clean);
    m_layout->addLayout(m_subLayout1, 0, 0, 1, 8);

    // row 1
    m_layout->addWidget(m_lb_Generation, 1, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_Generation, 1, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_Generation, 1, 7, 1, 1, 0);

    // row 2
    m_layout->addWidget(m_lb_Population, 2, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_Population, 2, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_Population, 2, 7, 1, 1, 0);

    // row 3
    m_layout->addWidget(m_lb_PSO1, 3, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_PSO1, 3, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_PSO1, 3, 7, 1, 1, 0);

    // row 4
    m_layout->addWidget(m_lb_PSO2, 4, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_PSO2, 4, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_PSO2, 4, 7, 1, 1, 0);

    // row 5
    m_layout->addWidget(m_lb_J, 5, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_J, 5, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_J, 5, 7, 1, 1, 0);

    // row 6
    m_layout->addWidget(m_lb_exp, 6, 0, 1, 1, 0);
    m_layout->addWidget(m_slider_exp, 6, 1, 1, 6, 0);
    m_layout->addWidget(m_lbValue_exp, 6, 7, 1, 1, 0);

    // row 7
    m_layout->addWidget(m_btn_StartOnce, 7, 0, 1, 1, 0);
    m_layout->addWidget(m_progress_once, 7, 1, 1, 7, 0);

    // row 8
    m_layout->addWidget(m_btn_StartMultiple, 8, 0, 1, 1, 0);
    m_layout->addWidget(m_progress_multiple, 8, 1, 1, 7, 0);

    m_layout->addWidget(m_txtConsole, 9, 0, 3, 8, 0);

    m_layout->addWidget(m_mainCanvasView, 0, 8, 12, 10, 0);

    setLayout(m_layout);
}

void MainWindow::compoentEventInit()
{
    connect(m_slider_Generation, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_Population, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_PSO1, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_PSO2, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_J, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));
    connect(m_slider_exp, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChange()));

    connect(m_btn_CaseData, SIGNAL(clicked()), this , SLOT(onBtnClick_CaseData()));
    connect(m_btn_TrainData, SIGNAL(clicked()), this , SLOT(onBtnClick_TrainData()));
    connect(m_btn_ConsoleClear, SIGNAL(clicked()), this, SLOT(onBtnClick_ConsoleClear()));
    connect(m_btn_StartOnce, SIGNAL(clicked()), this, SLOT(onBtnClick_PSO_Start()));
    connect(m_btn_clean, SIGNAL(clicked()), this, SLOT(onBtnClick_CleanAndRestart()));
    connect(m_btn_StartMultiple, SIGNAL(clicked()), this, SLOT(onBtnClick_PSO_MultipleStart()));

    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

void MainWindow::onSliderValueChange()
{
    QString strV1, strV2;
    if ( QObject::sender() == m_slider_PSO1 )
    {
        float v1 = (float)m_slider_PSO1->value() / 10.0;
        float v2 = 100.0 - v1;
        strV1.sprintf("%04.1f%%", v1);
        strV2.sprintf("%04.1f%%", v2);
        m_slider_PSO2->setValue(v2*10);
    }
    else if ( QObject::sender() == m_slider_PSO2 )
    {
        float v2 = (float)m_slider_PSO2->value() / 10.0;
        float v1 = 100.0 - v2;
        strV1.sprintf("%04.1f%%", v1);
        strV2.sprintf("%04.1f%%", v2);
        m_slider_PSO1->setValue(v1*10);
    }
    else
    {
        strV1.sprintf("%04.1f%%", (float)m_slider_PSO1->value() / 10.0);
        strV2.sprintf("%04.1f%%", (float)m_slider_PSO2->value() / 10.0);
    }

    m_lbValue_Generation->setText(QString::number(m_slider_Generation->value()));
    m_lbValue_Population->setText(QString::number(m_slider_Population->value()));
    m_lbValue_PSO1->setText(strV1);
    m_lbValue_PSO2->setText(strV2);
    m_lbValue_J->setText(QString::number(m_slider_J->value()));
    m_lbValue_exp->setText(QString::number(m_slider_exp->value()));
}

void MainWindow::onBtnClick_CaseData()
{
    QString strOutput = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));
    m_init = new CaseDataLoader(string(strOutput.toLocal8Bit()));

    switch ( m_init->initCheck() )
    {
    case FLS_NO_ERROR:
    {
        consoleAddMsg(QString("Load success."));
        consoleAddMsg(strOutput);
        consoleAddMsg(QString("\n"));
        break;
    }
    case FLS_FILE_PATH_INVAILD:
        consoleAddMsg(QString("File path error, the path must contain no Chinese letter"));
        return;
    default:
        consoleAddMsg(QString("Error, load fail."));
        return;
    }

    m_btn_CaseData->setEnabled(false);
    if ( !m_btn_CaseData->isEnabled() && !m_btn_TrainData->isEnabled() )
    {
        m_btn_StartOnce->setEnabled(true);
        m_btn_StartMultiple->setEnabled(true);
    }

    m_data->m_carPosition = m_init->getCarInitCoordinate();
    m_data->m_carAngle = m_init->getCarInitAngle();
    dataInit();
}

void MainWindow::onBtnClick_TrainData()
{
    QString strOutput = QFileDialog::getOpenFileName(this, QString("select input file"), "", QString("Text Files (*.txt)"));

    TrainDataLoader *loader = new TrainDataLoader();
    m_data->m_vTrainData = loader->load(string(strOutput.toLocal8Bit()));

    switch ( loader->checkStatus() )
    {
    case FLS_NO_ERROR:
        consoleAddMsg(QString("Load success."));
        consoleAddMsg(strOutput);
        consoleAddMsg(QString("\n"));
        m_btn_TrainData->setEnabled(false);
        break;
    case FLS_FILE_PATH_INVAILD:
        consoleAddMsg(QString("File path error, the path must contain no Chinese letter"));
        break;
    default:
        consoleAddMsg(QString("Error, load fail."));
        break;
    }

    m_btn_TrainData->setEnabled(false);
    if ( !m_btn_CaseData->isEnabled() && !m_btn_TrainData->isEnabled() )
    {
        m_btn_StartOnce->setEnabled(true);
        m_btn_StartMultiple->setEnabled(true);
    }
}

void MainWindow::onBtnClick_ConsoleClear()
{
    m_txtConsole->setPlainText(QString(""));
}

void MainWindow::onBtnClick_PSO_Start()
{
    setCurrentlyRunning();
    releaseThread();
    m_runThread = new QThread(this);
    m_PSO = new PSO();
    m_PSO->setParameter(m_slider_Generation->value(), m_slider_Population->value(), \
                        m_slider_PSO1->value(), m_slider_PSO2->value(), m_slider_J->value());
    m_PSO->setTrainData(m_data->m_vTrainData);
    connect(m_PSO, SIGNAL(PSO_Did_finsih()), this, SLOT(onPSO_finish()));
    connect(m_PSO, SIGNAL(PSO_Did_start()), this, SLOT(onPSO_Start()));
    connect(m_PSO, SIGNAL(PSO_error()), this, SLOT(onPSO_error()));
    m_PSO->moveToThread(m_runThread);
    m_runThread->start();
    m_PSO->run(1);
    m_progress_once->setMaximum(m_slider_Generation->value()*1);
}

void MainWindow::onBtnClick_PSO_MultipleStart()
{
    setCurrentlyRunning();
    releaseThread();
    m_runThread = new QThread(this);
    m_PSO = new PSO();
    m_PSO->setParameter(m_slider_Generation->value(), m_slider_Population->value(), \
                        m_slider_PSO1->value(), m_slider_PSO2->value(), m_slider_J->value());
    m_PSO->setTrainData(m_data->m_vTrainData);
    connect(m_PSO, SIGNAL(PSO_Did_finsih()), this, SLOT(onPSO_Multiplefinish()));
    connect(m_PSO, SIGNAL(PSO_Did_start()), this, SLOT(onPSO_MultipleStart()));
    connect(m_PSO, SIGNAL(PSO_error()), this, SLOT(onPSO_Multipleerror()));
    m_PSO->moveToThread(m_runThread);
    m_runThread->start();
    m_PSO->run(m_slider_exp->value());
    m_progress_multiple->setMaximum(m_slider_Generation->value()*m_slider_exp->value());
}

void MainWindow::onBtnClick_CleanAndRestart()
{
    m_btn_clean->setEnabled(false);
    setCurrentlyReady();
    m_data->m_vRecord.clear();
    m_data->m_vvMultipleRecord.clear();
    dataInit();
}

void MainWindow::onPSO_Start()
{
    consoleAddMsg(QString("PSO did Start"));
    m_timer->start(100);
}

void MainWindow::onPSO_error()
{
    consoleAddMsg(QString("PSO did error"));
    m_btn_clean->setEnabled(true);
}

void MainWindow::onPSO_finish()
{
    consoleAddMsg(QString("PSO did finish"));
    m_btn_clean->setEnabled(true);
    m_progress_once->setValue(m_progress_once->maximum());

    Particule p = m_PSO->getBestParticules()[0];
    TrainDataType type = m_data->m_vTrainData[0].dataType;

    CarObject *car = new CarObject(m_data);
    RBFN *network = new RBFN(p);

    consoleAddMsg(QString("=======以下為RBFN網路使用PSO演算法訓練出之最佳解的輸出結果======="));
    if ( type == TrainDataType_6D )
        consoleAddMsg(QString("(x, y)  distance(Front, Right, Left)  [RBFN output angle]"));
    else
        consoleAddMsg(QString("distance(Front, Right, Left)  [RBFN output angle]"));

    while ( true )
    {
        float F, R, L;

        carStatusRefresh(F, R, L);
        float angle = getRBFN_networkOutput(network, p, type, F, R, L);
        carMoveOnce(car, angle, true, true, F, R, L);

        if ( car->collisionCheck() == true )
        {
            consoleAddMsg(QString("碰撞發生，提前終止."));
            break;
        }
        if ( isCarReachEndZone() == true )
        {
            consoleAddMsg(QString("成功通過軌道！"));
            break;
        }
    }

    consoleAddMsg(QString("適應函數值： ") + QString::number(m_PSO->getBestFitnessValues()[0]));
    consoleAddMsg(QString("耗時： ") + convertTime(m_PSO->getExecutionTime()));
    consoleAddMsg(QString("平均誤差： ") + QString::number(m_PSO->getBsetAverageErrors()[0]));
    consoleAddMsg(QString("=======以上為RBFN網路使用PSO演算法訓練出之最佳解的輸出結果======="));
    consoleAddMsg(QString("\n"));

    delete car;
    delete network;
    allCompoentRefresh();
}

void MainWindow::onPSO_MultipleStart()
{
    consoleAddMsg(QString("PSO experiment did Start"));
    m_timer->start(100);
}

void MainWindow::onPSO_Multipleerror()
{
    consoleAddMsg(QString("PSO experiment did error"));
    m_btn_clean->setEnabled(true);
}

void MainWindow::onPSO_Multiplefinish()
{
    consoleAddMsg(QString("PSO experiment did finish"));
    m_btn_clean->setEnabled(true);
    m_progress_multiple->setValue(m_progress_once->maximum());

    QString strV1, strV2;
    float v1 = (float)m_slider_PSO1->value() / 10.0;
    float v2 = (float)m_slider_PSO2->value() / 10.0;
    strV1.sprintf("%04.1f%%", v1);
    strV2.sprintf("%04.1f%%", v2);

    int nExpTimes = m_slider_exp->value();

    consoleAddMsg(QString("Genetic Algorithm Experiment did finish running"));
    consoleAddMsg(QString("=============以下為PSO演算法運行 ") + QString::number(nExpTimes) + QString(" 次的輸出結果============="));
    consoleAddMsg(QString("實驗參數 [") + QString::number(m_slider_Generation->value()) + QString(", ") + \
                                         QString::number(m_slider_Population->value()) + QString(", ") + \
                                         strV1 + QString(", ") + \
                                         strV2 + QString(", ") + \
                                         QString::number(m_slider_J->value()) + QString("]"));
    consoleAddMsg(QString("[Best fitness], [Average Error], [是否成功通過軌道]"));

    vector<float> vAvgError = m_PSO->getBsetAverageErrors();
    vector<float> vBestFit = m_PSO->getBestFitnessValues();
    for ( int i=0 ; i < nExpTimes ; i++ )
    {
        QString msg = QString("");
        QString err, fit, pass;
        err.sprintf("%10.7f  ", vAvgError[i]);
        fit.sprintf("%10.7f   ", vBestFit[i]);
        pass = experimentCheckIsCarSuccessfullyPassTrack(i) ? QString("Yes") : QString("No ");
        msg += QString("[") + fit + QString("], ");
        msg += QString("[") + err + QString("], ");
        msg += QString("[") + pass + QString("]");
        consoleAddMsg(msg);
    }

    int nExecTime = m_PSO->getExecutionTime();
    consoleAddMsg(QString("全部執行時間: ") + convertTime(nExecTime));
    consoleAddMsg(QString("平均執行時間: ") + convertTime(nExecTime/nExpTimes));
    consoleAddMsg(QString("=============以上為PSO演算法運行 ") + QString::number(nExpTimes) + QString(" 次的輸出結果============="));
    allCompoentRefresh();
}

void MainWindow::onTimeOut()
{
    if ( m_PSO != NULL && m_PSO->isExperimentMode() )
    {
        m_progress_multiple->setValue(m_PSO->getProgress());
    }
    else if ( m_PSO != NULL && !m_PSO->isExperimentMode() )
    {
        m_progress_once->setValue(m_PSO->getProgress());
    }
}

void MainWindow::dataInit()
{
    if ( m_data->m_vTrackBorderInfo.empty() )
        m_data->m_vTrackBorderInfo = m_init->getTrackInfo();

    if ( m_data->m_vEndZoneBorderInfo.empty() )
        m_data->m_vEndZoneBorderInfo = m_init->getEndZoneInfo();

    m_data->m_carAngle = m_init->getCarInitAngle();
    m_data->m_carPosition = m_init->getCarInitCoordinate();

    m_mainCanvasView->setDataCenter(m_data);

    allCompoentRefresh();
}

void MainWindow::allCompoentRefresh()
{
    m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
    m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
    m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());

    m_mainCanvasView->repaint();
}

void MainWindow::consoleInit()
{
    QTextDocument *doc = m_txtConsole->document();

    // Monospaced Font
    QFont font = doc->defaultFont();
    font.setFamily("Courier New");
    doc->setDefaultFont(font);

    m_txtConsole->setReadOnly(true);
}

void MainWindow::consoleAddMsg(QString strMsg)
{
    if ( m_txtConsole->toPlainText().toStdString() == "" )
    {
        m_txtConsole->setPlainText(m_txtConsole->toPlainText() + strMsg);
    }
    else
    {
        if ( m_txtConsole->toPlainText().toStdString().back() == '\n' )
        {
            m_txtConsole->setPlainText(m_txtConsole->toPlainText() + strMsg);
        }
        else
        {
            m_txtConsole->setPlainText(m_txtConsole->toPlainText() + QString("\n") + strMsg);
        }
    }

    m_txtConsole->verticalScrollBar()->setValue(m_txtConsole->verticalScrollBar()->maximum());
}

bool MainWindow::isCarReachEndZone()
{
    float xMax=-1000, xMin=1000, yMax=-1000, yMin=1000;

    for ( vector<Point>::iterator it=m_data->m_vEndZoneBorderInfo.begin() ; it != m_data->m_vEndZoneBorderInfo.end() ; it++ )
    {
        Point pt = (*it);

        if ( pt.x > xMax )
            xMax = pt.x;

        if ( pt.x < xMin )
            xMin = pt.x;

        if ( pt.y > yMax )
            yMax = pt.y;

        if ( pt.y < yMin )
            yMin = pt.y;
    }

    if ( m_data->m_carPosition.x >= xMin && m_data->m_carPosition.x <= xMax )
    {
        if ( m_data->m_carPosition.y >= yMin && m_data->m_carPosition.y <= yMax )
            return true;
    }

    return false;
}

QString MainWindow::convertTime(const int microSecond)
{
    QString strHour, strMinute, strSecond, strResult;

    float ss = (int)((microSecond / 1000) % 60) + ((float)(microSecond % 1000) / 1000.0);
    int mm = (microSecond / 1000) / 60 % 60;
    int hh = (microSecond / 1000) / 3600 % 24;
    strHour.sprintf("%02d", hh);
    strMinute.sprintf("%02d", mm);
    strSecond.sprintf("%07.4f", ss);

    strResult = strHour + QString("小時 ") + strMinute + QString("分 ") + strSecond + QString("秒");

    return strResult;
}

void MainWindow::releaseThread()
{
    // Properly delete QThread
    // https://stackoverflow.com/a/31442217
    if ( m_runThread != NULL && m_PSO != NULL )
    {
        if ( m_PSO != NULL && m_PSO->isExperimentMode() )
        {
            disconnect(m_PSO, SIGNAL(PSO_Did_finsih()), this, SLOT(onPSO_Multiplefinish()));
            disconnect(m_PSO, SIGNAL(PSO_Did_start()), this, SLOT(onPSO_MultipleStart()));
            disconnect(m_PSO, SIGNAL(PSO_error()), this, SLOT(onPSO_Multipleerror()));
        }
        else if ( m_PSO != NULL && !m_PSO->isExperimentMode() )
        {
            disconnect(m_PSO, SIGNAL(PSO_Did_finsih()), this, SLOT(onPSO_finish()));
            disconnect(m_PSO, SIGNAL(PSO_Did_start()), this, SLOT(onPSO_Start()));
            disconnect(m_PSO, SIGNAL(PSO_error()), this, SLOT(onPSO_error()));
        }

        m_runThread->quit();
        m_runThread->wait();
        m_runThread->deleteLater();
        m_PSO->deleteLater();
    }
}

void MainWindow::setCurrentlyReady()
{
    if ( !m_btn_CaseData->isEnabled() && !m_btn_TrainData->isEnabled() )
    {
        m_btn_StartOnce->setEnabled(true);
        m_btn_StartMultiple->setEnabled(true);
    }

    m_slider_Generation->setEnabled(true);
    m_slider_Population->setEnabled(true);
    m_slider_PSO1->setEnabled(true);
    m_slider_PSO2->setEnabled(true);
    m_slider_J->setEnabled(true);
    m_slider_exp->setEnabled(true);

    if ( m_timer != NULL )
        m_timer->stop();
}

void MainWindow::setCurrentlyRunning()
{
    m_btn_StartOnce->setEnabled(false);
    m_btn_StartMultiple->setEnabled(false);
    m_slider_Generation->setEnabled(false);
    m_slider_Population->setEnabled(false);
    m_slider_PSO1->setEnabled(false);
    m_slider_PSO2->setEnabled(false);
    m_slider_J->setEnabled(false);
    m_slider_exp->setEnabled(false);
}

void MainWindow::carMoveOnce(CarObject *car, float angle, bool bMessageShow, bool bAddRecord, const float F, const float R, const float L)
{
    // 紀錄是空的，在 car move 前先加入目前位置的這一筆.
    if ( (bAddRecord == true) && (m_data->m_vRecord.size() < 1) )
    {
        Record rec = Record(m_data->m_carPosition, L, R, F, m_data->m_carWheelAngle.getDegree());
        m_data->m_vRecord.push_back(rec);
    }

    // 傳入的參數都是網路的輸出，直接拿來拼成輸出訊息.
    if ( bMessageShow == true )
    {
        TrainDataType type = m_data->m_vTrainData[0].dataType;
        QString strX, strY, strF, strR, strL, stra;
        QString msg = QString("");
        if ( type == TrainDataType_6D )
        {
            strX.sprintf("%+06.2f", m_data->m_carPosition.x);
            strY.sprintf("%+06.2f", m_data->m_carPosition.y);
            msg += QString("(") + strX + QString(",") + strY + QString(") ");
        }
        strF.sprintf("%05.2f", F);
        strR.sprintf("%05.2f", R);
        strL.sprintf("%05.2f", L);
        stra.sprintf("%+06.2f", angle);

        msg += QString("(") + strF + QString(",") + strR + QString(",") + strL + QString(") ");
        msg += QString("[angle: ") + stra + QString("]\n");

        consoleAddMsg(msg);
    }

    ( angle > 0 ) ? m_data->carTurnRight(angle) : m_data->carTurnLeft(angle);
    car->move();

    if ( bAddRecord == true )
    {
        Record rec = Record(m_data->m_carPosition, L, R, F, m_data->m_carWheelAngle.getDegree());
        m_data->m_vRecord.push_back(rec);
    }
}

// 更新車體感測器與軌道邊界的交點、同時取得三個距離.
void MainWindow::carStatusRefresh(float &disFront, float &disRight, float &disLeft)
{
    Geometry geoObj;

    m_data->m_intersectPt_L = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() + 45.0);
    m_data->m_intersectPt_R = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree() - 45.0);
    m_data->m_intersectPt_F = m_data->nextIntersectionPoint(m_data->m_carAngle.getDegree());

    disFront = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_F);
    disRight = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_R);
    disLeft  = geoObj.getDistance(m_data->m_carPosition, m_data->m_intersectPt_L);
}

float MainWindow::getRBFN_networkOutput(RBFN *network, Particule networksetting, TrainDataType dataType, float F, float R, float L)
{
    InputVector input(dataType);
    int idx=0;
    if ( dataType == TrainDataType_6D )
    {
        input.aryElement[idx++] = (m_data->m_carPosition.x-30.0) / 40.0;
        input.aryElement[idx++] = (m_data->m_carPosition.y-30.0) / 40.0;
    }
    input.aryElement[idx++] = (F-40.0) / 40.0;
    input.aryElement[idx++] = (R-40.0) / 40.0;
    input.aryElement[idx++] = (L-40.0) / 40.0;

    return network->getResult(input, networksetting) * 40.0;
}

bool MainWindow::experimentCheckIsCarSuccessfullyPassTrack(int i)
{
    Particule p = m_PSO->getBestParticules()[i];
    TrainDataType type = m_data->m_vTrainData[0].dataType;
    RBFN *network = new RBFN(p);
    CarObject *car = new CarObject(m_data);
    m_data->m_vRecord.clear();
    m_data->m_carAngle = m_init->getCarInitAngle();
    m_data->m_carPosition = m_init->getCarInitCoordinate();

    while ( true )
    {
        float F, R, L;

        carStatusRefresh(F, R, L);
        float angle = getRBFN_networkOutput(network, p, type, F, R, L);
        carMoveOnce(car, angle, false, true, F, R, L);
        m_data->m_vvMultipleRecord.push_back(m_data->m_vRecord);

        if ( car->collisionCheck() == true )
        {
            delete car;
            delete network;
            return false;
        }
        else if ( isCarReachEndZone() == true )
        {
            break;
        }
    }

    delete car;
    delete network;
    return true;
}
