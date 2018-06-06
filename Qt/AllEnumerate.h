#ifndef ALLENUMERATE_H
#define ALLENUMERATE_H

#ifndef EN_QUAD
#define EN_QUAD
typedef enum enQuadrant
{
    Quadrant_First, Quadrant_Second, Quadrant_Third, Quadrant_Fourth,
    X_Axis_Positive, X_Axis_Negative, Y_Axis_Positive, Y_Axis_Negative
}Quadrant;
#endif // EN_QUAD

#ifndef EN_ANGLE
#define EN_ANGLE
typedef enum enAngleType
{
    ANGLE_TYPE_DEGREE, ANGLE_TYPE_RADIANS
}angleType;
#endif // EN_ANGLE

#ifndef EN_LOADSTATUS
#define EN_LOADSTATUS
typedef enum enFileLoadStatue
{
    FLS_NO_ERROR,
    FLS_ERROR_TRACK_NOT_CLOSE,
    FLS_ERROR_CAR_INIT_INFO,
    FLS_ERROR_DESTINATION_INFO,
    FLS_ERROR_TRACK_EMPTY,
    FLS_FILE_PATH_INVAILD
}fileLoadStatue;
#endif // EN_LOADSTATUS

#ifndef EN_TRAINDATA
#define EN_TRAINDATA
typedef enum enTrainDataType
{
    TrainDataType_4D=4, TrainDataType_6D=6
}TrainDataType;
#endif // EN_TRAINDATA

#endif // ALLENUMERATE_H
