#pragma once

#include <chrono>
#include <ctime> 
#include <iostream>
#include <vector>
#include <sstream>

#define TRACE(...) kj::MyTraceImplTmp(__LINE__, __FILE__, __VA_ARGS__)

namespace kj
{   
    static std::string timePointAsString(const std::chrono::system_clock::time_point& tp) 
    {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::string ts = std::ctime(&t);
        
        // get rid off new line 
        ts.resize(ts.size()-1);
        
        return ts;
    }

    static std::vector<std::string> splitString(const std::string& s, char delimiter)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter))
        {
            tokens.push_back(token);
        }
        
        return tokens;
    }

    template <typename ...Args>
    static void MyTraceImplTmp(int line, const char* filePath, Args&& ...args)
    {   
        auto currentTime = std::chrono::system_clock::now();
        auto splittedFilePath = splitString(std::string(filePath), '/');
        std::string fileName = splittedFilePath[splittedFilePath.size()-1];

        std::ostringstream stream;   

        stream << "[" << timePointAsString(currentTime) << "] " << fileName << "(" << line << "): ";
        (stream << ... << std::forward<Args>(args)) << '\n';

        std::cout << stream.str();
    }
}