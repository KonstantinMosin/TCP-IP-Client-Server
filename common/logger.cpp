#include "logger.h"

Logger::Logger(string path, Level level) : f(path, std::ios::app), level(level) {
    if (!f.is_open()) {
        // ?
    }
    interpretation[Level::DEBUG] = "DEBUG";
    interpretation[Level::INFO] = "INFO";
    interpretation[Level::WARNING] = "WARNING";
    interpretation[Level::ERROR] = "ERROR";
    interpretation[Level::CRITICAL] = "CRITICAL";
}

Logger::~Logger() {
    if (f.is_open()) {
        f.close();
    }
}

void Logger::setLevel(Level level) {
    this->level = level;
}

void Logger::write(string message, Level level) {
    if (f.is_open() && this->level <= level) {
        f << interpretation[level] << ": " << message;
    }
}