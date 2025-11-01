#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <mutex>
#include <chrono>
#include <iomanip>

enum class LogLevel { DEBUG = 0, INFO = 1, WARNING = 2, ERROR = 3, FATAL = 4 };

class Logger {
private:
  static std::unique_ptr<Logger> instance;
  static std::mutex mutex_;

  std::ofstream logFile;
  LogLevel currentLogLevel;
  bool consoleOutput;
  bool fileOutput;
  std::mutex logMutex;

  Logger();

  std::string getCurrentTimestamp();
  std::string logLevelToString(LogLevel level);
  void writeLog(LogLevel level, const std::string& message);

public:
  static Logger& getInstance();

  Logger(Logger& other) = delete;
  void operator=(const Logger&) = delete;

  ~Logger();

  void setLogLevel(LogLevel level);
  void enableConsoleOutput(bool enable);
  void enableFileOutput(bool enable, const std::string& filename = "engine.log");

  void debug(const std::string& message);
  void info(const std::string& message);
  void warning(const std::string& message);
  void error(const std::string& message);
  void fatal(const std::string& message);

  template <typename... Args>
  void debug(const std::string& format, Args... args);

  template <typename... Args>
  void info(const std::string& format, Args... args);

  template <typename... Args>
  void warning(const std::string& format, Args... args);

  template <typename... Args>
  void error(const std::string& format, Args... args);

  template <typename... Args>
  void fatal(const std::string& format, Args... args);

private:
  template <typename T, typename... Args>
  std::string formatString(const std::string& format, T value, Args... args);
  std::string formatString(const std::string& format);
};

template <typename... Args>
void Logger::debug(const std::string& format, Args... args) {
  writeLog(LogLevel::DEBUG, formatString(format, args...));
}

template <typename... Args>
void Logger::info(const std::string& format, Args... args) {
  writeLog(LogLevel::INFO, formatString(format, args...));
}

template <typename... Args>
void Logger::warning(const std::string& format, Args... args) {
  writeLog(LogLevel::WARNING, formatString(format, args...));
}

template <typename... Args>
void Logger::error(const std::string& format, Args... args) {
  writeLog(LogLevel::ERROR, formatString(format, args...));
}

template <typename... Args>
void Logger::fatal(const std::string& format, Args... args) {
  writeLog(LogLevel::FATAL, formatString(format, args...));
  std::exit(EXIT_FAILURE);
}

template <typename T, typename... Args>
std::string Logger::formatString(const std::string& format, T value, Args... args) {
  std::ostringstream oss;
  std::size_t start = 0;
  std::size_t openBrace = format.find('{');

  if (openBrace == std::string::npos) {
    return format;
  }

  std::size_t closeBrace = format.find('}', openBrace);

  if (closeBrace == std::string::npos) {
    return format;
  }

  oss << format.substr(start, openBrace - start);
  oss << value;
  oss << formatString(format.substr(closeBrace + 1), args...);

  return oss.str();
}

#define LOG_DEBUG(msg) Logger::getInstance().debug(msg)
#define LOG_INFO(msg) Logger::getInstance().info(msg)
#define LOG_WARNING(msg) Logger::getInstance().warning(msg)
#define LOG_ERROR(msg) Logger::getInstance().error(msg)
#define LOG_FATAL(msg) Logger::getInstance().fatal(msg)

#define LOG_DEBUG_F(format, ...) Logger::getInstance().debug(format, __VA_ARGS__)
#define LOG_INFO_F(format, ...) Logger::getInstance().info(format, __VA_ARGS__)
#define LOG_WARNING_F(format, ...) Logger::getInstance().warning(format, __VA_ARGS__)
#define LOG_ERROR_F(format, ...) Logger::getInstance().error(format, __VA_ARGS__)
#define LOG_FATAL_F(format, ...) Logger::getInstance().fatal(format, __VA_ARGS__)
