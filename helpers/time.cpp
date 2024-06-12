#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>


std::string getTodayDate() {
    std::time_t now = std::time(nullptr);

    std::tm* localTime = std::localtime(&now);

    std::ostringstream oss;
    oss << std::setfill('0') 
        << std::setw(2) << (localTime->tm_year % 100) << '-' 
        << std::setw(2) << (localTime->tm_mon + 1) << '-'    
        << std::setw(2) << localTime->tm_mday;                

    return oss.str();
}