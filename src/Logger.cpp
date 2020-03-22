#include "Logger.h"
#include <iostream>
#include <fstream>

void Logger::Log(Level level, const std::string& msg) noexcept
{
    std::string header;
    switch (level)
    {
    case Level::Info:
    {
        header = "[INFO] ";
        std::cout << header << msg << std::endl;
        break;
    }
    case Level::Warning:
    {
        header = "[WARNING] ";
        std::cout << header << msg << std::endl;
        break;
    }
    case Level::Error:
    {
        header = "[ERROR] ";
        std::cerr << header << msg << std::endl;
        break;
    }
    }
    std::fstream fout("log.txt", std::fstream::app | std::fstream::out);
    if (!fout.good())
        std::cerr << "[ERROR] Cannot open log file \"log.txt\"!" << std::endl;
    fout << header << msg << std::endl;
    fout.close();
}