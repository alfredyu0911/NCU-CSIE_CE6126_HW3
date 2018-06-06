#include "TrainDataLoader.h"
#include <algorithm>
#include <fstream>

vector<DataContent> TrainDataLoader::load(string fileFullPath)
{
    ifstream file(fileFullPath, ios::in);

    vector<DataContent> vData;
    vData.clear();

    if ( !file )
    {
        m_status = FLS_FILE_PATH_INVAILD;
    }
    m_status = FLS_ERROR_TRACK_NOT_CLOSE;

    bool bFirst = true;

    string strLine;
    while ( getline(file, strLine) )
    {
        if ( bFirst )
        {
            m_type = (TrainDataType)count(strLine.begin(), strLine.end(), '.');
            bFirst = false;
        }

        if ( count(strLine.begin(), strLine.end(), '.') != (int)m_type )
        {
            vData.clear();
            return vData;
        }

        DataContent data = parserStringLine(strLine, " ");

        vData.push_back(data);
    }

    m_status = FLS_NO_ERROR;
    return vData;
}

DataContent TrainDataLoader::parserStringLine(string strInput, string strGap)
{
    DataContent data;
    data.dataType = m_type;

    if ( m_type == TrainDataType_6D )
    {
        data.positionX = stod(strInput.substr(0, strInput.find(strGap)));
        strInput.erase(0, strInput.find(strGap)+strGap.length());

        data.positionY = stod(strInput.substr(0, strInput.find(strGap)));
        strInput.erase(0, strInput.find(strGap)+strGap.length());
    }

    data.distanceFront = stod(strInput.substr(0, strInput.find(strGap)));
    strInput.erase(0, strInput.find(strGap)+strGap.length());

    data.distanceRight = stod(strInput.substr(0, strInput.find(strGap)));
    strInput.erase(0, strInput.find(strGap)+strGap.length());

    data.distanceLeft = stod(strInput.substr(0, strInput.find(strGap)));
    strInput.erase(0, strInput.find(strGap)+strGap.length());

    data.wheelAngle = stod(strInput);

    return data;
}
