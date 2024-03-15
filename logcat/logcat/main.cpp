#include <iostream>

#include "DateTime.h"
#include "LogFilter.h"
#include "LogData.h"
#include "LogParser.h"

enum filterOption {filterById, filterByTimeRange};

int main(int argc, char* argv[]) {
    bool filterFlag = false;
    bool levelFlag = false;

    LogParser lp;

    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-filter") == 0) {
            i++;
            filterFlag = true;
            if (std::strcmp(argv[i], "0") == 0) {
                lp.setOptionZeroIsSet(true);
                lp.setPid(std::stoi(argv[++i]));
                lp.setVid(std::stoi(argv[++i]));
            }
            else if (std::strcmp(argv[i], "1") == 0) {
                lp.setOptionOneIsSet(true);
                lp.setStartTime(argv[++i]);
                lp.setEndTime(argv[++i]);
            }
            else {
                std::cout << "Invalid filter option" << std::endl;
            }
        }
        else if (std::strcmp(argv[i], "-level") == 0) {
            i++;
            levelFlag = true;
            lp.setLevelIsSet(true);
            if (std::strcmp(argv[i], "0") == 0) {
                lp.setLevel(std::stoi(argv[++i]));
            } else if (std::strcmp(argv[i], "1") == 0) {
                lp.setLevel(std::stoi(argv[++i]));
            }
            else if (std::strcmp(argv[i], "2") == 0) {
                lp.setLevel(std::stoi(argv[++i]));
            }
            else if (std::strcmp(argv[i], "3") == 0) {
                lp.setLevel(std::stoi(argv[++i]));
            }
            else if (std::strcmp(argv[i], "4") == 0) {
                lp.setLevel(std::stoi(argv[++i]));
            }
            else {
                lp.setLevelIsSet(false);
                std::cout << "Invalid filter option" << std::endl;
            }
        }
    }

    lp.parseLogFile("logcat.txt");
	return 0;
}