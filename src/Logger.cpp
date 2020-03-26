#include "Logger.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>

void Logger::Log(Level level, const std::string& msg) noexcept
{
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string header = "<" + std::string(std::ctime(&now)) + "> ";
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
    std::fstream fout("log.txt", std::fstream::app | std::fstream::out);
    if (!fout.good())
    {
        now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cerr << "<" << std::ctime(&now) << "> [ERROR] Cannot open log file \"log.txt\"!" << std::endl;
    }
    else
    {
        fout << header << msg << std::endl;
        fout.close();
    }
}