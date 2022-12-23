#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <map>

using std::string;

enum class Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        CRITICAL
    };

class Logger {
public:
    Logger(string path, Level level = Level::INFO);
    ~Logger();
    void setLevel(Level level);
    void write(string message, Level level = Level::INFO);

private:
    std::fstream f;
    Level level;

    std::map<Level, string> interpretation;
};

#endif
