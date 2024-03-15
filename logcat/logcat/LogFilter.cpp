#include "LogFilter.h"

std::map<int, std::string> tag = {
        {0, "FATAL"},
        {1, "ERROR"},
        {2, "WARNING"},
        {3, "INFO"},
        {4, "DEBUG"}
};

bool LogFilter::filterById(const LogData& entry, int pid, int vid) const {
    if (entry.m_pid == pid && entry.m_vid == vid) {
        return true;
    }
    return false;
}

bool LogFilter::filterByTimeRange(const LogData& entry, std::string startTime, std::string endTime) const {
    std::string str = entry.m_time;
    str.erase(std::remove(str.begin(), str.end(), ':'), str.end());
    startTime.erase(std::remove(startTime.begin(), startTime.end(), ':'), startTime.end());
    endTime.erase(std::remove(endTime.begin(), endTime.end(), ':'), endTime.end());

    return (std::stoi(str) >= std::stoi(startTime) && std::stoi(str) <= std::stoi(endTime));
}

bool LogFilter::filterByLogLevel(const LogData& entry, int level) const {
    //std::cout << tag[level] << std::endl;
    if (entry.m_logLevel == tag[level]) {
        return true;
    }
    return false;
}
