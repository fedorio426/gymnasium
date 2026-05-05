#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

// C++ program to implement a basic logging system.

#include <ctime>   // localtime
#include <iomanip> // put_time
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;
using namespace chrono;

// Enum to represent log levels
enum class LogLevel { DBG, INF, WRN, ERR, CRITICAL, NOP };

class Logger {
public:
    // Constructor: Opens the log file in append mode
    Logger(string filename="")
    {
        if (filename.empty()) filename = "log.txt";
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            cerr << "Error opening log file." << endl;
        }
    }

    // Destructor: Closes the log file
    ~Logger() { logFile.close(); }

    // Logs a message with a given log level
    void log(LogLevel level, const string& message)
    {
        // Get current time
        auto my_time = system_clock::to_time_t(system_clock::now());

        // Create log entry
        ostringstream logEntry;
        if (level != LogLevel::NOP)
        logEntry << "[" << put_time(localtime(&my_time), "%Y-%m-%d %H:%M:%S") << "] "
                 << levelToString(level) << ": ";
        logEntry << message << endl;

        // Output to console !!! ???
        cout << logEntry.str();

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str();
            logFile.flush(); // Ensure immediate write to file
        }
    }

private:
    ofstream logFile; // File stream for the log file

    // Converts log level to a string for output
    string levelToString(LogLevel level)
    {
        switch (level) {
        case LogLevel::DBG:  return "DBG";
        case LogLevel::INF:  return "INF";
        case LogLevel::WRN:  return "WRN";
        case LogLevel::ERR:  return "ERR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default:                 return "";
        }
    }
};


#endif // LOGGER_H_INCLUDED
