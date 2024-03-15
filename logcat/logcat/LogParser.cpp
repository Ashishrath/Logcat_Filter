#include "LogParser.h"
#include "DateTime.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

void LogParser::setPid(int p) {
    pid = p;
}

void LogParser::setVid(int v) {
    vid = v;
}

void LogParser::setLevel(int l) {
    level = l;
}

void LogParser::setOptionZeroIsSet(bool opt) {
    optionZeroIsSet = opt;
}

void LogParser::setOptionOneIsSet(bool opt) {
    optionOneIsSet = opt;
}

void LogParser::setLevelIsSet(bool opt) {
    levelIsSet = opt;
}

void LogParser::setStartTime(const std::string time) {
    startTime = time;
}

void LogParser::setEndTime(const std::string time) {
    endTime = time;
}

LogData LogParser::parseLogEntry(const std::string& line) {
    std::string date = line.substr(0, 10);
    std::string time = line.substr(11, 8);
    size_t endPosOfPid = line.find(' ', 20);
    int pid = std::stoi(line.substr(20, endPosOfPid));
    size_t endPosOfVid = line.find(' ', endPosOfPid + 1);
    size_t startOfLevel = line.find(' ', endPosOfVid + 1);
    int vid = std::stoi(line.substr(endPosOfPid + 1, endPosOfVid - endPosOfPid));
    size_t endOfLevel = line.find(' ', endPosOfVid + 1);
    std::string level = line.substr(endPosOfVid + 1, endOfLevel - endPosOfVid - 1);
    std::string message = line.substr(endOfLevel + 1);
    LogData entry(date, time, pid, vid, level, message);

    return entry;
}

std::vector<LogData> LogParser::parseLogFile(const std::string& filename) {
    std::vector<LogData> logEntries;

    try
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return logEntries;
        }

        std::string line;
        while (std::getline(file, line)) {
            LogData entry = parseLogEntry(line);
            logEntries.push_back(entry);
        }

        file.close();
    }
    catch (const std::exception&)
    {
        std::cout << "An error occured while opening " << filename << " file." << std::endl;
    }
    
    try
    {
        const std::string filteredLogFileName = "FilteredLogs/FilteredLog[" + currTime.getCurrentDateTime() + "].txt";
        std::ofstream logFile(filteredLogFileName);

        if (!logFile.is_open()) {
            std::cout << "Unable to open log file" << std::endl;
            throw "An error occures while creating/opening the log file.";
        }

        for (const auto& entry : logEntries) {
            if (levelIsSet == true && filter.filterByLogLevel(entry, level)) {
                logFile << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
            }

            else if (optionZeroIsSet == true && filter.filterById(entry, pid, vid)) {
                logFile << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
            }

            else if (optionOneIsSet == true && filter.filterByTimeRange(entry, startTime, endTime)) {
                logFile << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
            }
            
            
        }
        logFile.close();
        std::cout << "Logs have been written to file '" << filteredLogFileName << "'" << std::endl;
    }
    catch (const char *err) 
    {
        std::cout << err << std::endl;
    }
    

    return logEntries;
}