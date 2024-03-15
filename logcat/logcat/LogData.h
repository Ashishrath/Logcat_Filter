#ifndef LOGDATA_H
#define LOGDATA_H

#include <iostream>


class LogData {
public:
	std::string m_date;
	std::string m_time;
	int m_pid;
	int m_vid;
	std::string m_logLevel;
	std::string m_message;

	LogData() {}
	LogData(const std::string date, const std::string time, int pid, int vid, const std::string logLevel, const std::string message) : m_date(date), m_time(time), m_pid(pid), m_vid(vid), m_logLevel(logLevel), m_message(message) {}

	void displayData();
};

#endif
