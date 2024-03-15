#ifndef LOGFILTER_H
#define LOGFILTER_H

#include <iostream>
#include <string>
#include <map>
#include "LogData.h"

/*std::map<int, std::string> tag = {
        {0, "FATAL"},
        {1, "ERROR"},
        {2, "WARNING"},
        {3, "INFO"},
        {4, "DEBUG"}
};*/

class LogFilter {
public:
    bool filterById(const LogData& entry, int pid, int vid) const;

    bool filterByTimeRange(const LogData& entry, std::string startTime, std::string endTime) const;

    bool filterByLogLevel(const LogData& entry, int level) const;
};

#endif#pragma once
