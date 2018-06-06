//
//  CarObject.hpp
//  FuzzySystemTest
//
//  Created by Alfred on 2018/3/28.
//  Copyright © 2018年 Alfred. All rights reserved.
//

#ifndef CarObject_hpp
#define CarObject_hpp

#include "SystemDataCenter.h"

class CarObject
{
public:
    CarObject(SystemDataCenter *data) { m_data = data; }
    virtual ~CarObject();
    
    void move();

    bool collisionCheck();

    float m_lastFuzzyRuleResult_wheelAngle;
    
protected:
    CarObject();
    SystemDataCenter *m_data;
};

#endif /* CarObject_hpp */
