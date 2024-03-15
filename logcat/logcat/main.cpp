#include <iostream>
#include <string>
#include <ctime>
#include <chrono>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

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
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H_%M_%S", &local_now);
        return buffer;
    }
};

class LogFilter {
private:
    int pid;
    int vid;
    std::string startTime;
    std::string endTime;
public:
    LogFilter() {}
    LogFilter(int pid, int vid) : pid(pid), vid(vid) {}
    LogFilter(const std::string& startTime, const std::string& endTime)
        : startTime(startTime), endTime(endTime) {}

    bool filterById(const LogData& entry, int pid, int vid) const {
        if (entry.m_pid == pid && entry.m_vid == vid) {
            return true;
        }
        return false;
    }

    bool filterByTimeRange(const LogData& entry, std::string startTime, std::string endTime) const {
        std::string str = entry.m_time;
        str.erase(std::remove(str.begin(), str.end(), ':'), str.end());
        startTime.erase(std::remove(startTime.begin(), startTime.end(), ':'), startTime.end());
        endTime.erase(std::remove(endTime.begin(), endTime.end(), ':'), endTime.end());
       
        return (std::stoi(str) >= std::stoi(startTime) && std::stoi(str) <= std::stoi(endTime));
    }
};

class LogParser {
private:
    LogFilter filter;
    int pid, vid;
    bool optionZeroIsSet = false, optionOneIsSet = false;
    std::string startTime, endTime;
    DateTime currTime;

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
public:

    void setPid(int p) {
        pid = p;
    }

    void setVid(int v) {
        vid = v;
    }

    void setOptionZeroIsSet(bool opt) {
        optionZeroIsSet = opt;
    }

    void setOptionOneIsSet(bool opt) {
        optionOneIsSet = opt;
    }

    void setStartTime(const std::string time) {
        startTime = time;
    }

    void setEndTime(const std::string time) {
        endTime = time;
    }

    std::vector<LogData> parseLogFile(const std::string& filename) {
        std::vector<LogData> logEntries;

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

        const std::string filteredLogFileName = "FilteredLogs/FilteredLog[" + currTime.getCurrentDateTime() + "].txt";
        //std::cout << filteredLogFileName << std::endl;
        std::ofstream logFile(filteredLogFileName);

        if (!logFile.is_open()) {
            std::cout << "Unable to open log file" << std::endl;
        }

        for (const auto& entry : logEntries) {
            if (optionZeroIsSet == true && filter.filterById(entry, pid, vid)) {
                logFile << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
            }

            if (optionOneIsSet == true && filter.filterByTimeRange(entry, startTime, endTime)) {
                logFile << entry.m_date << " " << entry.m_time << " " << entry.m_pid << " " << entry.m_vid << " " << "[" << entry.m_logLevel << "]" << /* entry.tag << */": " << entry.m_message << std::endl;
            }      
        }
        logFile.close();
        std::cout << "Logs have been written to file '"<< filteredLogFileName << "'" << std::endl;

        return logEntries;
    }
};

int main(int argc, char* argv[]) {
    DateTime d;
    //std::cout << argc << std::endl;

    //int option;
    bool filterFlag = false;
    bool levelFlag = false;

    LogParser lp;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-filter") == 0) {
            i++;
            filterFlag = true;
            if (std::strcmp(argv[i], "0") == 0) {
                //option = 0;
                //lp.option = 0;
                lp.setOptionZeroIsSet(true);
                lp.setPid(std::stoi(argv[++i]));

                lp.setVid(std::stoi(argv[++i]));
            }
            else if (std::strcmp(argv[i], "1") == 0) {
                //lp.option = 1;
                lp.setOptionOneIsSet(true);
                lp.setStartTime(argv[++i]);

                lp.setEndTime(argv[++i]);
            }
            else {
                std::cout << "Invalid filter option" << std::endl;
            }
        }
        //std::cout << argv[i] << std::endl;
    }

    std::string date = (d.getCurrentDateTime()).substr(0, 10);
	std::string time = (d.getCurrentDateTime()).substr(11);
    // std::cout << date << std::endl;
	// std::cout << time << std::endl;
    //LogProcessor processor("logcat.txt", 1234, 111, time, time);
    //processor.processLog();
	//tag t;
	//LogData l(date, time, 1234, 11, "DEBUG", "Test Message");
	//l.displayData();
    
    

    
    //std::cout << "Select filter option: " << std::endl;
    //std::cin >> option;

    /*if (filterFlag && option == 0) {
        lp.option = 0;
        std::cout << "Enter pid: " << std::endl;
        std::cin >> lp.pid;
        
        std::cout << "Enter vid: " << std::endl;
        std::cin >> lp.vid;
    }
    if (filterFlag && option == 1) {
        lp.option = 1;
        std::cout << "Enter startTime: " << std::endl;
        std::cin >> lp.startTime;
        std::cout << "Enter endTime: " << std::endl;
        std::cin >> lp.endTime;
    }*/

    lp.parseLogFile("logcat.txt");
	return 0;
}