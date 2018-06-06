#ifndef CANVAS_H
#define CANVAS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QWidget>
#include <vector>

#include "SystemDataCenter.h"
#include "AllStructure.h"

class Canvas : public QWidget
{
public:
    Canvas();
    ~Canvas();

    void paintEvent(QPaintEvent *);

    void setDataCenter(SystemDataCenter *dc) { m_data = dc; }

protected:
    Point m_ptCar, m_ptL, m_ptR, m_ptF;
    vector<Record> m_vRecord;
    vector<vector<Record>> m_vvRecord;

    SystemDataCenter *m_data;

    void getData();

    void drawBoundary(QPainter &painter);
    void setPainter(QPainter &painter);
    void drawTrackBorder(QPainter &painter);
    void drawEndZoneBorder(QPainter &painter);
    void drawCar(QPainter &painter);
    void drawCarSensor(QPainter &painter);
    void drawRecord(QPainter &painter);
};

#endif // CANVAS_H
