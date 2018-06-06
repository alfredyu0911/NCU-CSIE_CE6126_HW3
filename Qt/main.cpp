#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(1000, 625);
    w.setWindowTitle(QString("計算型智慧 作業三 Y1060147 余紹畇"));
    w.show();

    return a.exec();
}
