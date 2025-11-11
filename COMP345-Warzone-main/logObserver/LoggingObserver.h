#pragma once

#include <string>
#include <memory>

class ILoggable {
public:
    virtual std::string stringToLog() const = 0;
};

class Observer{
    public:
        virtual void update(ILoggable& loggable) = 0;

};

class LogObserver : Observer{
    private:
        std::string filePath;

    public:
        LogObserver();
        LogObserver(std::string filePath);
        LogObserver(const LogObserver& copy);
        void update(ILoggable& loggable);

};

class Subject {
public:
    virtual void attach(std::shared_ptr<LogObserver> pObserver) = 0;
    virtual void detach() = 0;
    virtual void notify(ILoggable& loggable) const = 0;
};

void testLoggingObserver();