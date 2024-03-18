#include "DateTime.h"

std::string DateTime::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_now;
    localtime_s(&local_now, &now_c);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H_%M_%S", &local_now);
    return buffer;
}
