#include "LogData.h"

void LogData::displayData() {
	std::cout << m_date << ' ' <<
		m_time << ' ' <<
		m_pid << ' ' <<
		m_vid << ' ' <<
		m_logLevel << ' ' <<
		m_message << ' ' << std::endl;
}