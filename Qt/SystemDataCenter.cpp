#include "SystemDataCenter.h"

SystemDataCenter::SystemDataCenter()
{
    m_vTrackBorderInfo.clear();
    m_vEndZoneBorderInfo.clear();
    m_vRecord.clear();
    m_vvMultipleRecord.clear();
}

Point SystemDataCenter::nextIntersectionPoint(float dDegreeToCheck)
{
    Point pt_nearest;
    float distance_nearest = 10E23;

    Degree degreeToCheck = Degree(dDegreeToCheck);
    Geometry geoObj;
    Quadrant aq = degreeToCheck.getQuadrant();

    vector<Point>::iterator it=m_vTrackBorderInfo.begin();
    vector<Point>::iterator it2=it++;
    while ( it != m_vTrackBorderInfo.end() && m_vTrackBorderInfo.size() > 0 )
    {
        bool flag = false;

        if ( it2 == m_vTrackBorderInfo.end() )
        {
            it2 = m_vTrackBorderInfo.begin();
            flag = true;
        }

        Point border_pt1=(*it), border_pt2=(*it2), pt_intersection;
        if ( geoObj.checkIntersect(border_pt1, border_pt2, m_carPosition, degreeToCheck) )
        {
            pt_intersection = geoObj.getIntersection();
            switch (aq)
            {
                case Quadrant_First:
                {
                    if ( pt_intersection.x > m_carPosition.x && pt_intersection.y > m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case Quadrant_Second:
                {
                    if ( pt_intersection.x < m_carPosition.x && pt_intersection.y > m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case Quadrant_Third:
                {
                    if ( pt_intersection.x < m_carPosition.x && pt_intersection.y < m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case Quadrant_Fourth:
                {
                    if ( pt_intersection.x > m_carPosition.x && pt_intersection.y < m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case X_Axis_Positive:
                {
                    if ( pt_intersection.x > m_carPosition.x )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case X_Axis_Negative:
                {
                    if ( pt_intersection.x < m_carPosition.x )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case Y_Axis_Positive:
                {
                    if ( pt_intersection.y > m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }
                case Y_Axis_Negative:
                {
                    if ( pt_intersection.y < m_carPosition.y )
                    {
                        float dis = geoObj.getDistance(m_carPosition, pt_intersection);
                        if ( dis < distance_nearest )
                        {
                            pt_nearest = pt_intersection;
                            distance_nearest = dis;
                        }

                    }
                    break;
                }

                default:
                    break;
            }
        }
        it++;
        it2++;

        if ( flag )
            break;
    }

    return pt_nearest;
}
