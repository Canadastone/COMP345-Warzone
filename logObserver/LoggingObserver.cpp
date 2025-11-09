#include "LoggingObserver.h"
#include <string>
#include <fstream>
#include <iostream>

LogObserver::LogObserver() {
    this->filePath = "./log.txt";
}
LogObserver::LogObserver(std::string filePath){
    this->filePath = filePath;
}

LogObserver::LogObserver(const LogObserver& copy) {
    this->filePath = copy.filePath;
}

void LogObserver::update(ILoggable& loggable){
    std::string toWrite = loggable.stringToLog();

    std::ofstream file(this->filePath, std::ios::out | std::ios::app);
    file << toWrite << std::endl;
    file << "----------------------------------------------------------------------------------" << std::endl;
    file.close();
}