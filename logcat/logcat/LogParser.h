#ifndef LOGPARSER_H
#define LOGPARSER_H

#include <iostream>
#include <vector>
#include "LogFilter.h"
#include "LogData.h"
#include "DateTime.h"

class LogParser {
private:
    LogFilter filter;
    int pid{ 0 }, vid{ 0 }, level{ 0 };
    bool optionZeroIsSet = false, optionOneIsSet = false, levelIsSet = false;
    std::string startTime{ "" }, endTime{ "" };
    DateTime currTime;

    LogData parseLogEntry(const std::string& line);
public:
    LogParser() {}

    void setPid(int p);

    void setVid(int v);

    void setLevel(int l);

    void setOptionZeroIsSet(bool opt);

    void setOptionOneIsSet(bool opt);

    void setLevelIsSet(bool opt);

    void setStartTime(const std::string time);

    void setEndTime(const std::string time);

    std::vector<LogData> parseLogFile(const std::string& filename);
};

#endif
