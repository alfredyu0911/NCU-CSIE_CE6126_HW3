#ifndef SYSTEMDATACENTER_H
#define SYSTEMDATACENTER_H

#include <vector>

#include "AllStructure.h"

class SystemDataCenter
{
public:
    SystemDataCenter();

    Point nextIntersectionPoint(float dDegreeToCheck);

    vector<Point> m_vTrackBorderInfo, m_vEndZoneBorderInfo;

    Point m_carPosition, m_intersectPt_L, m_intersectPt_R, m_intersectPt_F;
    Degree m_carAngle, m_carWheelAngle;

    void carTurnRight(Degree degree)
    {
        if ( degree.getDegree() > WHEEL_ANGLE_RIGHT_MAX )
            m_carWheelAngle = Degree(WHEEL_ANGLE_RIGHT_MAX);
        else
            m_carWheelAngle = Degree(degree.getDegree());
    }

    void carTurnLeft(Degree degree)
    {
        if ( degree.getDegree() < WHEEL_ANGLE_LEFT_MAX )
            m_carWheelAngle = Degree(WHEEL_ANGLE_LEFT_MAX);
        else
            m_carWheelAngle = Degree(degree.getDegree());
    }

    vector<DataContent> m_vTrainData;

    vector<Record> m_vRecord;
    vector<vector<Record>> m_vvMultipleRecord;
};

#endif // SYSTEMDATACENTER_H
