#include <QDebug>
#include <QPainter>
#include "Canvas.h"
#include "AllConstant.hpp"

Canvas::Canvas()
{
    m_data = NULL;
    m_vRecord.clear();
}

Canvas::~Canvas()
{
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 0.25, Qt::SolidLine, Qt::RoundCap));

    drawBoundary(painter);

    // 視為尚未初始化完成.
    if ( m_data == NULL )
        return;

    if ( m_data->m_vTrackBorderInfo.size() < 1 )
        return;

    getData();

    setPainter(painter);
    drawTrackBorder(painter);
    drawEndZoneBorder(painter);

    drawRecord(painter);

    drawCar(painter);
    drawCarSensor(painter);
}

void Canvas::drawRecord(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(QColor(20, 20, 20), 0.1, Qt::DotLine, Qt::RoundCap));

    if ( m_vvRecord.size() > 0 )
    {
        for ( vector<vector<Record>>::iterator it0=m_vvRecord.begin() ; it0 != m_vvRecord.end() ; it0++ )
        {
            if ( ((vector<Record>)(*it0)).size() > 1 )
            {
                vector<Record> vRec = (*it0);
                for ( vector<Record>::iterator it=vRec.begin() ; it != vRec.end() ; it++ )
                {
                    if ( it == vRec.begin() )
                        continue;

                    Record recStart = (*(it-1));
                    Record recEnd = (*it);
                    Point ptStart = recStart.pt;
                    Point ptEnd = recEnd.pt;
                    painter.drawLine(QPointF(ptStart.x, ptStart.y), QPointF(ptEnd.x, ptEnd.y));
                }
            }
        }
    }
    else
    {
        if ( m_vRecord.size() > 1 )
        {
            for ( vector<Record>::iterator it=m_vRecord.begin() ; it != m_vRecord.end() ; it++ )
            {
                if ( it == m_vRecord.begin() )
                    continue;

                Record recStart = (*(it-1));
                Record recEnd = (*it);
                Point ptStart = recStart.pt;
                Point ptEnd = recEnd.pt;
                painter.drawLine(QPointF(ptStart.x, ptStart.y), QPointF(ptEnd.x, ptEnd.y));
            }
        }
    }

    painter.restore();
}

void Canvas::drawBoundary(QPainter &painter)
{
    float w=width(), h=height();
    float gap = 1;

    painter.drawLine(gap, gap, w-gap, gap);
    painter.drawLine(w-gap, gap, w-gap, h-gap);
    painter.drawLine(w-gap, h-gap, gap, h-gap);
    painter.drawLine(gap, h-gap, gap, gap);
}

void Canvas::setPainter(QPainter &painter)
{
    float xMax=-1000, xMin=1000, yMax=-1000, yMin=1000;

    painter.setPen(QPen(QColor(20, 20, 20), 0.25, Qt::SolidLine, Qt::RoundCap));

    for ( vector<Point>::iterator it=m_data->m_vTrackBorderInfo.begin() ; it != m_data->m_vTrackBorderInfo.end() ; it++ )
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

    float gap = 3;

    float xRange = (xMax-xMin) + gap*2;
    float yRange = (yMax-yMin) + gap*2;

    float w=width(), h=height();

    QMatrix m;
    m.translate((fabs(xMin)+gap)*w/xRange, h-((fabs(yMin)+gap))*h/yRange);
    m.scale(1, -1);
    painter.setMatrix(m);

    painter.scale(w/xRange, h/yRange);
}

void Canvas::drawTrackBorder(QPainter &painter)
{
    painter.setPen(QPen(QColor(0, 21, 255), 0.25, Qt::SolidLine, Qt::RoundCap));

    vector<Point>::iterator it1 = m_data->m_vTrackBorderInfo.begin();
    vector<Point>::iterator it2 = it1++;

    Point pt1, pt2;
    while ( it1 != m_data->m_vTrackBorderInfo.end() )
    {
        pt1 = (*it1);
        pt2 = (*it2);

        painter.drawLine(pt1.x, pt1.y, pt2.x, pt2.y);

        it1++;
        it2++;
    }

    pt2 = *m_data->m_vTrackBorderInfo.begin();
    painter.drawLine(QPointF(pt1.x, pt1.y), QPointF(pt2.x, pt2.y));
}

void Canvas::drawEndZoneBorder(QPainter &painter)
{
    if ( m_data->m_vEndZoneBorderInfo.size() < 1 )
        return;

    painter.setPen(QPen(QColor(255, 0, 8), 0.25, Qt::SolidLine, Qt::RoundCap));

    vector<Point>::iterator it1 = m_data->m_vEndZoneBorderInfo.begin();
    vector<Point>::iterator it2 = it1++;

    Point pt1, pt2;
    while ( it1 != m_data->m_vEndZoneBorderInfo.end() )
    {
        pt1 = (*it1);
        pt2 = (*it2);

        painter.drawLine(QPointF(pt1.x, pt1.y), QPointF(pt2.x, pt2.y));

        it1++;
        it2++;
    }

    pt2 = *m_data->m_vEndZoneBorderInfo.begin();
    painter.drawLine(QPointF(pt1.x, pt1.y), QPointF(pt2.x, pt2.y));
}

void Canvas::drawCar(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(QColor(50, 205, 50), 0.25, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(QColor(50, 205, 50)));
    painter.drawEllipse(QPointF(m_ptCar.x, m_ptCar.y), CAR_INIT_LENGHT, CAR_INIT_LENGHT);
    painter.restore();
}

void Canvas::drawCarSensor(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(QColor(189, 63, 0), 0.1, Qt::SolidLine, Qt::RoundCap));
    painter.setBrush(QBrush(QColor(189, 63, 0)));
    float radius = 0.3;

    // left
    painter.drawEllipse(QPointF(m_data->m_intersectPt_L.x, m_data->m_intersectPt_L.y), radius, radius);

    // right
    painter.drawEllipse(QPointF(m_data->m_intersectPt_R.x, m_data->m_intersectPt_R.y), radius, radius);

    // front
    painter.drawEllipse(QPointF(m_data->m_intersectPt_F.x, m_data->m_intersectPt_F.y), radius, radius);

    painter.setPen(QPen(QColor(189, 63, 0), 0.1, Qt::DotLine, Qt::RoundCap));
    painter.drawLine(QPointF(m_ptCar.x, m_ptCar.y), QPointF(m_ptL.x, m_ptL.y));
    painter.drawLine(QPointF(m_ptCar.x, m_ptCar.y), QPointF(m_ptR.x, m_ptR.y));
    painter.drawLine(QPointF(m_ptCar.x, m_ptCar.y), QPointF(m_ptF.x, m_ptF.y));

    painter.restore();
}

void Canvas::getData()
{
    m_vRecord = m_data->m_vRecord;
    m_vvRecord = m_data->m_vvMultipleRecord;

    m_ptCar = m_data->m_carPosition;
    m_ptL = m_data->m_intersectPt_L;
    m_ptR = m_data->m_intersectPt_R;
    m_ptF = m_data->m_intersectPt_F;
}
