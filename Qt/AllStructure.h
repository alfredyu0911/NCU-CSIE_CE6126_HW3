#ifndef ALLSTRUCT_H
#define ALLSTRUCT_H

#include <cmath>
#include <vector>
#include <cstdlib>

#include "AllEnumerate.h"
#include "AllConstant.hpp"

using namespace std;

static inline long mod(long a, long b) { return ( a % b + b ) % b; }

#ifndef ST_POINT
#define ST_POINT
typedef struct stPoint
{
    float x, y;

    stPoint()
    {
        x = CAR_INIT_POSITION_X;
        y = CAR_INIT_POSITION_Y;
    }

    stPoint(float dx, float dy)
    {
        x = dx;
        y = dy;
    }

}Point, Coordinate;
#endif //ST_POINT

#ifndef ST_ANGLE
#define ST_ANGLE
typedef struct stAngleWithDegree
{
protected:
    float angle;

public:
    stAngleWithDegree() { angle = 0.0; }
    stAngleWithDegree(float degree) { angle = degree; }

    float getDegree() { return angle; }
    float getRadians() { return angle * (M_PI / 180.0); }
    void setAsDegree(float degree) { angle = degree; }
    void setAsRadians(float radians) { angle = radians * (180.0 / M_PI); }

    Quadrant getQuadrant()
    {
        double degree = mod(angle, 360);

        if ( degree == 0.0 )
        {
            return X_Axis_Positive;
        }
        else if ( degree > 0.0 && degree < 90.0 )
        {
            return Quadrant_First;
        }
        else if ( degree == 90.0 )
        {
            return Y_Axis_Positive;
        }
        else if ( degree > 90.0 && degree < 180.0 )
        {
            return Quadrant_Second;
        }
        else if ( degree == 180.0 )
        {
            return X_Axis_Negative;
        }
        else if ( degree > 180.0 && degree < 270.0 )
        {
            return Quadrant_Third;
        }
        else if ( degree == 270.0 )
        {
            return Y_Axis_Negative;
        }
        else
        {
            return Quadrant_Fourth;
        }
    }
}AngleWithDegree, Degree;
#endif //ST_ANGLE

#ifndef ST_GEO
#define ST_GEO
typedef struct stGeometry
{
private:
    Point ptIntersection;
public:
    bool checkIntersect(Point ptStart, Point ptEnd, Point car, Degree carAngle)
    {
        // y - y' = m(x - x')  ->  y = m(x - x') + y'
        //
        // m1(x-x1')+y1' = m2(x-x2')+y2'  ->  m1x-m1x1'+y1' = m2x-m2x2'+y2'
        // ->  (m1-m2)x = m1x1' - m2x2' - y1' +y2'
        // ->  x = (m1x1' - m2x2' - y1' + y2') / (m1-m2)

        float m_line = (ptEnd.y - ptStart.y) / (ptEnd.x - ptStart.x);
        float m_car = tan(carAngle.getRadians());

        if ( ptEnd.x == ptStart.x && carAngle.getDegree() == 90.0 )
        {
            return false;
        }
        else if ( ptEnd.x == ptStart.x )
        {
            // border is vertical, car is not
            float X = ptStart.x;
            float Y = m_car*(X - car.x) + car.y;

            if ( Y > max(ptStart.y, ptEnd.y) || Y < min(ptStart.y, ptEnd.y) )
                return false;

            ptIntersection.x = X;
            ptIntersection.y = Y;
            return true;
        }
        else if ( carAngle.getDegree() == 90.0 )
        {
            // the car is vertical, border is not
            float X = car.x;
            float Y = m_line*(X - ptStart.x) + ptStart.y;

            if ( X > max(ptStart.x, ptEnd.x) || X < min(ptStart.x, ptEnd.x) )
                return false;

            ptIntersection.x = X;
            ptIntersection.y = Y;
            return true;
        }
        else if ( ptStart.y == ptEnd.y )
        {
            float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
            float Y = ptStart.y;

            if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) )
            {
                ptIntersection.x = X;
                ptIntersection.y = Y;
                return true;
            }
        }
        else
        {
            float X = (m_line*ptStart.x - m_car*car.x - ptStart.y + car.y) / (m_line - m_car);
            float Y = m_car*(X - car.x) + car.y;

            if ( X <= max(ptStart.x, ptEnd.x) && X >= min(ptStart.x, ptEnd.x) &&
                 Y <= max(ptStart.y, ptEnd.y) && Y >= min(ptStart.y, ptEnd.y)   )
            {
                ptIntersection.x = X;
                ptIntersection.y = Y;
                return true;
            }
        }

        return false;
    }

    Point getIntersection()
    {
        return ptIntersection;
    }

    float getDistance(Point pt1, Point pt2)
    {
        return sqrt(pow(pt2.x-pt1.x, 2.0) + pow(pt2.y-pt1.y, 2.0));
    }

    // https://stackoverflow.com/a/6853926
    float minDistanceFromPointToLine(float ptX, float ptY, float line1X, float line1Y, float line2X, float line2Y)
    {
        float A = ptX - line1X;
        float B = ptY - line1Y;
        float C = line2X - line1X;
        float D = line2Y - line1Y;

        float dot = A * C + B * D;
        float len_sq = C * C + D * D;
        float param = -1;

        //in case of 0 length line
        if (len_sq != 0)
            param = dot / len_sq;

        float xx, yy;

        if (param < 0)
        {
            xx = line1X;
            yy = line1Y;
        }
        else if (param > 1)
        {
            xx = line2X;
            yy = line2Y;
        }
        else
        {
            xx = line1X + param * C;
            yy = line1Y + param * D;
        }

        float dx = ptX - xx;
        float dy = ptY - yy;

        return sqrt(dx * dx + dy * dy);
    }

}Geometry;
#endif //ST_GEO

#ifndef ST_DATACONTENT
#define ST_DATACONTENT
typedef struct stDataContent
{
    TrainDataType dataType;
    double positionX, positionY;
    double distanceFront, distanceRight, distanceLeft;
    double wheelAngle;
}DataContent;
#endif //ST_DATACONTENT

#ifndef ST_RECORD
#define ST_RECORD
typedef struct stRecord
{
    Point pt;
    float distanceL, distanceR, distanceF;
    float wheelAngle;
    stRecord(Point p, float L, float R, float F, float deg)
    {
        pt = p;
        distanceL = L;
        distanceR = R;
        distanceF = F;
        wheelAngle = deg;
    }

}Record;
#endif //ST_RECORD

#ifndef ST_PARTICULE
#define ST_PARTICULE
typedef struct stParticule
{
    vector<float> position;         // 目前位置.
    vector<float> velocity;         // 目前速度向量.
    int geneDimension;              // 序列維度.
    float fitness;                  // 適性值.
    vector<float> localBest;        // 個體最佳序列.
    vector<float> globalBest;       // 群體最佳序列.
    float fitness_LB, fitness_GB;

    stParticule()
    {
        geneDimension = 0;
        position.clear();
        velocity.clear();
        localBest.clear();
        globalBest.clear();
        fitness = fitness_LB = fitness_GB = 9999.9;
    }

    void initGeneSerialArray(int dimensionOfChromosome)
    {
        if ( dimensionOfChromosome > 0 )
        {
            geneDimension = dimensionOfChromosome;
            position.resize(geneDimension);
            velocity.resize(geneDimension);
            localBest.resize(geneDimension);
            globalBest.resize(geneDimension);
        }
    }

    // 不寫成 copy constructor 是因為 stParticule push 進 std::vector 裡面會有奇怪的問題.
    // 時間關係，先這樣解.
    void copyFromInput(const stParticule &p)
    {
        geneDimension = p.geneDimension;
        position.resize(geneDimension);
        velocity.resize(geneDimension);
        localBest.resize(geneDimension);
        globalBest.resize(geneDimension);
        for ( int i=0 ; i < geneDimension ; i++ )
        {
            position[i] = p.position[i];
            velocity[i] = p.velocity[i];
            localBest[i] = p.localBest[i];
            globalBest[i] = p.globalBest[i];
        }
        fitness = p.fitness;
        fitness_LB = p.fitness_LB;
        fitness_GB = p.fitness_GB;
    }

    void updateGlobalBest(const stParticule &p)
    {
        geneDimension = p.geneDimension;
        globalBest.resize(geneDimension);
        for ( int i=0 ; i < geneDimension ; i++ )
        {
            globalBest[i] = p.position[i];
        }
        fitness_GB = p.fitness;
    }

    void updateLocalBest(const stParticule &p)
    {
        geneDimension = p.geneDimension;
        localBest.resize(geneDimension);
        for ( int i=0 ; i < geneDimension ; i++ )
        {
            localBest[i] = p.position[i];
        }
        fitness_LB = p.fitness;
    }

    void checkVelocity()
    {
        float v = 0.0;
        for ( int i=0 ; i < geneDimension ; i++ )
        {
            v += pow(velocity[i], 2.0);
        }
        v = sqrt(v);
        if ( v > PARTICULE_MAX_VELOCITY )
        {
            float ratio = PARTICULE_MAX_VELOCITY / v;
            for ( int i=0 ; i < geneDimension ; i++ )
            {
                velocity[i] *= ratio;
            }
        }
    }

    void randomlyAssignSerialAndVelocity()
    {
        for ( int i=0 ; i < geneDimension ; i++ )
        {
            // -1 ~ 1
            float random = (double)rand()*2 / RAND_MAX + (-1);
            position[i] = random;
            random = (double)rand()*2 / RAND_MAX + (-1);
            velocity[i] = random;
        }
    }

    ~stParticule()
    {
        position.clear();
        velocity.clear();
        localBest.clear();
        globalBest.clear();
    }
}Particule;
#endif //ST_PARTICULE

#ifndef ST_INPUTVECTOR
#define ST_INPUTVECTOR
typedef struct stInputVector
{
//    float *aryElement;  // 向量元素.
    vector<float> aryElement;
    int nDimension;     // 陣列維度.

    stInputVector() { aryElement.clear(); nDimension = 0; }

    stInputVector(TrainDataType type)
    {
        switch ( type )
        {
        case TrainDataType_4D:
            nDimension = 3;
            break;
        case TrainDataType_6D:
            nDimension = 5;
            break;
        }
        aryElement.resize(nDimension);
    }

    ~stInputVector()
    {
        aryElement.clear();
    }
}InputVector;
#endif //ST_INPUTVECTOR

#endif // ALLSTRUCT_H
