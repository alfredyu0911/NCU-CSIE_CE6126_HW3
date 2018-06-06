//
//  CaseDataLoader.h
//
//  Created by Alfred on 2018/5/20.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef CaseDataLoader_h
#define CaseDataLoader_h

#include <vector>

#include "AllStructure.h"

using namespace std;

class CaseDataLoader
{
public:
    CaseDataLoader(string fileFullPath);
    
    fileLoadStatue initCheck();
    
    Coordinate getCarInitCoordinate();
    Degree getCarInitAngle();
    
    vector<Point> getTrackInfo();
    vector<Point> getEndZoneInfo();

protected:
    CaseDataLoader();
    
private:
    fileLoadStatue m_status;
    Coordinate m_carCoordinate;
    Degree m_carAngle;
    vector<Point> m_vTrackInfo;
    vector<Point> m_vEndZoneInfo;
    
    bool loadData(string fileFullPath);
    bool parserCarCoordinate(string str);
    bool parserEndZone(string strStart, string strEnd);
    Point stringToPoint(string strPoint, string strGap);
};

#endif /* CaseDataLoader_h */
