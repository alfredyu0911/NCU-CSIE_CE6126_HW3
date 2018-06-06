#ifndef TRAINDATALOADER_H
#define TRAINDATALOADER_H

#include <string>
#include <vector>

#include "AllEnumerate.h"
#include "AllStructure.h"

using namespace std;

class TrainDataLoader
{
public:
    vector<DataContent> load(string fileFullPath);
    fileLoadStatue checkStatus() { return m_status; }

protected:
    TrainDataType m_type;
    fileLoadStatue m_status;
    DataContent parserStringLine(string strInput, string strGap);
};

#endif // TRAINDATALOADER_H
