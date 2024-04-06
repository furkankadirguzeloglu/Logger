# Logger

A logging library that is used for quickly and succinctly writing log messages with various log levels and colors to either the console or a file.

## Features

- Define log levels: NORMAL, DEBUG, INFO, WARNING, ERROR, CRITICAL, TRACE, VERBOSE, AUDIT, FATAL
- Assign custom color and prefix for each log level
- Option to log to console and file
- Colorful console output using ANSI color codes
- Use of mutex for thread safety

## Usage 
```cpp
#include "Logger.hpp"

int main() {
    Logger logger;

    // Set log level
    logger.setLogLevel(LogLevel::NORMAL);

    // Set console and file writing options
    logger.setWriteToConsole(true);
    logger.setWriteToFile(true);

    // Set log file name
    logger.setFileName("mylog.txt");

    // Set log colors
    logger.setLogColor(LogLevel::INFO, "#00ff00");
    logger.setLogColorRGB(LogLevel::ERROR, 255, 0, 0);
    
    // Write log message
    logger.writeLog("This is a normal message.");
    logger.writeLog("This is a debug message.", LogLevel::DEBUG);
    logger.writeLog("This is an info message.", LogLevel::INFO);
    logger.writeLog("This is a warning message.", LogLevel::WARNING);
    logger.writeLog("This is an error message.", LogLevel::ERROR);
    logger.writeLog("This is a critical message.", LogLevel::CRITICAL);
    logger.writeLog("This is a trace message.", LogLevel::TRACE);
    logger.writeLog("This is a verbose message.", LogLevel::VERBOSE);
    return 0;
}
```

## License
This project is licensed under the terms of the MIT license. See the [LICENSE](https://raw.githubusercontent.com/furkankadirguzeloglu/Logger/main/LICENSE) file for details.
