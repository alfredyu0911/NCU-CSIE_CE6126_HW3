//
//  CaseDataLoader.cpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#include <fstream>
#include "CaseDataLoader.h"

using namespace std;

CaseDataLoader::CaseDataLoader(string fileFullPath)
{
    m_status = FLS_NO_ERROR;
    m_carCoordinate = Coordinate();
    m_vTrackInfo.clear();
    m_vEndZoneInfo.clear();
    
    loadData(fileFullPath);
}

bool CaseDataLoader::loadData(string fileFullPath)
{
    ifstream road(fileFullPath, ios::in);
    string strCarCoordinateInfo, strDestinationAreaLeftTop, strDestinationAreaRightBottom, strBorderInfo;
    
    if ( !road )
    {
        m_status = FLS_FILE_PATH_INVAILD;
    }

    if ( !(road >> strCarCoordinateInfo) )
    {
        m_status = FLS_ERROR_CAR_INIT_INFO;
        return false;
    }
    
    if ( false == parserCarCoordinate(strCarCoordinateInfo) )
    {
        m_status = FLS_ERROR_CAR_INIT_INFO;
        return false;
    }
    
    if ( !(road >> strDestinationAreaLeftTop) )
    {
        m_status = FLS_ERROR_DESTINATION_INFO;
        return false;
    }
    
    if ( !(road >> strDestinationAreaRightBottom) )
    {
        m_status = FLS_ERROR_DESTINATION_INFO;
        return false;
    }
    
    if ( false == parserEndZone(strDestinationAreaLeftTop, strDestinationAreaRightBottom) )
    {
        m_status = FLS_ERROR_DESTINATION_INFO;
        return false;
    }
    
    m_vTrackInfo.clear();
    while ( road >> strBorderInfo )
        m_vTrackInfo.push_back(stringToPoint(strBorderInfo, ","));
    
    if ( m_vTrackInfo.size() < 1 )
    {
        m_status = FLS_ERROR_TRACK_EMPTY;
        return false;
    }
    
    // the input info should be form as a close track.
    // hence, the start point must be equal to end point.
    Point startPt = (*m_vTrackInfo.begin());
    Point endPt = (*(--m_vTrackInfo.end()));
    if ( startPt.x != endPt.x || startPt.y != endPt.y )
    {
        m_status = FLS_ERROR_TRACK_NOT_CLOSE;
        return false;
    }
    
    return m_status == FLS_NO_ERROR ? true : false;
}

fileLoadStatue CaseDataLoader::initCheck()
{
    return m_status;
}

bool CaseDataLoader::parserEndZone(string strStart, string strEnd)
{
    if ( strStart.find(',') == string::npos )
        return false;
    
    if ( strEnd.find(',') == string::npos )
        return false;
    
    Point leftTop, rightBottom;
    
    leftTop = stringToPoint(strStart, ",");
    rightBottom = stringToPoint(strEnd, ",");
    
    Point p1 = Point(leftTop.x, leftTop.y);
    Point p2 = Point(rightBottom.x, leftTop.y);
    Point p3 = Point(rightBottom.x, rightBottom.y);
    Point p4 = Point(leftTop.x, rightBottom.y);
    
    m_vEndZoneInfo.clear();
    m_vEndZoneInfo.push_back(p1);
    m_vEndZoneInfo.push_back(p2);
    m_vEndZoneInfo.push_back(p3);
    m_vEndZoneInfo.push_back(p4);
    
    return true;
}

bool CaseDataLoader::parserCarCoordinate(string str)
{
    if ( str.find(',') == string::npos )
        return false;
    
    m_carCoordinate.x = stod(str.substr(0, str.find(',')));
    str.erase(0, str.find(',')+1);
    
    if ( str.find(',') == string::npos )
        return false;
    
    m_carCoordinate.y = stod(str.substr(0, str.find(',')));
    str.erase(0, str.find(',')+1);
    
    float degree = stod(str.substr(0, str.length()));
    m_carAngle.setAsDegree(degree);
    
    return true;
}

Coordinate CaseDataLoader::getCarInitCoordinate()
{
    return m_carCoordinate;
}

Degree CaseDataLoader::getCarInitAngle()
{
    return m_carAngle;
}

vector<Point> CaseDataLoader::getTrackInfo()
{
    return m_vTrackInfo;
}

vector<Point> CaseDataLoader::getEndZoneInfo()
{
    return m_vEndZoneInfo;
}

Point CaseDataLoader::stringToPoint(string strPoint, string strGap)
{
    Point pt_res;
    pt_res.x = 0.0;
    pt_res.y = 0.0;
    
    if ( strPoint.find(strGap) == string::npos )
        return pt_res;
    
    pt_res.x = stod(strPoint.substr(0, strPoint.find(strGap)));
    strPoint.erase(0, strPoint.find(strGap)+strGap.length());
    
    pt_res.y = stod(strPoint.substr(0, strPoint.find(strGap)));
    
    return pt_res;
}
