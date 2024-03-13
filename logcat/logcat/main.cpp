#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <fstream>

enum tag { FATAL, ERROR, WARNING, INFO, DEBUG };
enum filterOption {filterById, filterByTimeRange};

class LogData {
public:
	std::string m_date;
	std::string m_time;
	int m_pid;
	int m_vid;
	std::string m_logLevel;
	std::string m_message;
	
public:
    LogData() {}
	LogData(const std::string date, const std::string time, int pid, int vid, const std::string logLevel, const std::string message) : m_date(date), m_time(time), m_pid(pid), m_vid(vid), m_logLevel(logLevel), m_message(message){}

    /*std::string getDate() {
        return m_date;
    }
    std::string getTime() {
        return m_time;
    }*/
	void displayData() {
		std::cout << m_date << ' ' <<
			m_time << ' ' <<
			m_pid << ' ' <<
			m_vid << ' ' <<
			m_logLevel << ' ' <<
			m_message << ' ' << std::endl;
	}
};

class DateTime {
public:
    std::string getCurrentDateTime() {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm local_now;
        localtime_s(&local_now, &now_c);
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &local_now);
        return buffer;
    }
};

class LogFilter {
public:
    LogFilter() {}
    LogFilter(int pid, int vid) : pid(pid), vid(vid) {}
    //LogFilter(const std::string& startTime, const std::string& endTime)
    //    : startTime(startTime), endTime(endTime) {}

    bool filterById(const LogData& entry, int pid, int vid) const {
        if (entry.m_pid == pid && entry.m_vid == vid) {
            return true;
        }
        return false;
    }

    bool filterByTimeRange(const LogData& entry, std::string startTime, std::string endTime) const {
        // Todo: time comparision
        //if(entry.m_time >= startTime && entry.m_time <= endTime) {
        //    return true;
        //}
        //return false;
    }

private:
    int pid;
    int vid;
    std::string startTime;
    std::string endTime;
};

class LogParser {
private:
    LogFilter filter;
public:
    std::vector<LogData> parseLogFile(const std::string& filename) {
        std::vector<LogData> logEntries;

        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file: " << filename << std::endl;
            return logEntries;
        }

        std::string line;
        while (std::getline(file, line)) {
            //std::cout << line << std::endl;
            LogData entry = parseLogEntry(line);
            logEntries.push_back(entry);
        }

        file.close();

        int option;
        std::cout << "Select filter option: " << std::endl;
        std::cin >> option;

        for (const auto& entry : logEntries) {
            //std::cout << entry.m_date << std::endl;
            if (option == 0) {
                int pid, vid;
                std::cout << "Enter pid: " << std::endl;
                std::cin >> pid;
                std::cout << "Enter vid: " << std::endl;
                std::cin >> vid;
                

                if (filter.filterById(entry, pid, vid)) {
                    // Output or process filtered log entries
                    std::cout << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
                }
            }
            
            if (option == 1) {
                std::string startTime, endTime;
                std::cout << "Enter startTime: " << std::endl;
                std::cin >> startTime;
                std::cout << "Enter endTime: " << std::endl;
                std::cin >> endTime;
                if (filter.filterByTimeRange(entry, startTime, endTime)) {
                    std::cout << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
                }
            }
            
        }

        return logEntries;
    }

private:
    LogData parseLogEntry(const std::string& line) {
        std::string date = line.substr(0, 10);
        std::string time = line.substr(11, 8);
        size_t endPosOfPid = line.find(' ', 20);
        int pid = std::stoi(line.substr(20, endPosOfPid));
        //std::cout << endPosOfPid << std::endl;
        //std::cout << "pid: " << pid << std::endl;
        size_t endPosOfVid = line.find(' ', endPosOfPid + 1);
        //std::cout << endPosOfVid<< std::endl;
        size_t startOfLevel = line.find(' ', endPosOfVid + 1);
        int vid = std::stoi(line.substr(endPosOfPid + 1, endPosOfVid - endPosOfPid));
        //std::string v = line.substr(endPosOfPid + 1, endPosOfVid - endPosOfPid);

        //std::cout << "vid: " << v << std::endl;
        size_t endOfLevel = line.find(' ', endPosOfVid + 1);
        std::string level = line.substr(endPosOfVid + 1, endOfLevel - endPosOfVid - 1);
        std::string message = line.substr(endOfLevel + 1);
        LogData entry(date, time, pid, vid, level, message);
 
        return entry;
    }
};

/*class LogProcessor {
public:
    LogProcessor(const std::string& filename, int pid, int tid, const std::string& startTime, const std::string& endTime)
     : filename(filename), filter(pid, tid, startTime, endTime) {}

    void processLog() {
        LogParser parser;
        std::vector<LogData> logEntries = parser.parseLogFile(filename);

        //for (const auto& entry : logEntries) {
            //if (filter.filter(entry)) {
                // Output or process filtered log entries
                // std::cout << entry.getDate() << " " << entry.m_time << " " << static_cast<int>(entry.logLevel) << " " << entry.tag << ": " << entry.message << std::endl;
            //}
        //}
    }

private:
    std::string filename;
    LogFilter filter;
};*/



int main() {
	std::string s;
	//std::getline(std::cin, s);
    DateTime d;

    std::string date = (d.getCurrentDateTime()).substr(0, 10);
	std::string time = (d.getCurrentDateTime()).substr(11);
    // std::cout << date << std::endl;
	// std::cout << time << std::endl;
    //LogProcessor processor("logcat.txt", 1234, 111, time, time);
    //processor.processLog();
	//tag t;
	//LogData l(date, time, 1234, 11, "DEBUG", "Test Message");
	//l.displayData();
    LogParser lp;
    lp.parseLogFile("logcat.txt");
	return 0;
}