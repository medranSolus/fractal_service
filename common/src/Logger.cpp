#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sys/stat.h>

void Logger::Log(Level level, const std::string& msg) noexcept
{
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto time = std::localtime(&now);
    std::string time_label = "<" + std::to_string(time->tm_mday) +
        "." + std::to_string(time->tm_mon) + "." + std::to_string(time->tm_year + 1900) +
        " " + std::to_string(time->tm_hour) + ":" + std::to_string(time->tm_min) + "> ";
    std::string header = time_label;
    switch (level)
    {
    case Level::Info:
    {
        header += "[INFO] ";
        std::cout << header << msg << std::endl;
        break;
    }
    case Level::Warning:
    {
        header += "[WARNING] ";
        std::cout << header << msg << std::endl;
        break;
    }
    case Level::Error:
    {
        header += "[ERROR] ";
        std::cerr << header << msg << std::endl;
        break;
    }
    }
    chmod("log.txt", 0666);
    std::fstream fout("log.txt", std::fstream::app | std::fstream::out);
    if (!fout.good())
        std::cerr << time_label << "[ERROR] Cannot open log file \"log.txt\"!" << std::endl;
    else
    {
        fout << header << msg << std::endl;
        fout.close();
    }
}