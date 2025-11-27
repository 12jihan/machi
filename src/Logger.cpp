#include "../include/Logger.hpp"

std::unique_ptr<Logger> Logger::instance = nullptr;
std::mutex Logger::mutex_;

Logger::Logger() : currentLogLevel(LogLevel::INFO), consoleOutput(true), fileOutput(false) {}

Logger::~Logger() {
  if (logFile.is_open()) {
    logFile.close();
  }
}

Logger& Logger::getInstance() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (instance == nullptr) {
    instance = std::unique_ptr<Logger>(new Logger());
  }
  return *instance;
}

void Logger::setLogLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(logMutex);
  currentLogLevel = level;
}

void Logger::enableConsoleOutput(bool enable) {
  std::lock_guard<std::mutex> lock(logMutex);
  consoleOutput = enable;
}

void Logger::enableFileOutput(bool enable, const std::string& filename) {
  std::lock_guard<std::mutex> lock(logMutex);

  if (enable) {
    if (logFile.is_open()) {
      logFile.close();
    }
    logFile.open(filename, std::ios::app);
    fileOutput = logFile.is_open();
  } else {
    if (logFile.is_open()) {
      logFile.close();
    }
    fileOutput = false;
  }
}

std::string Logger::getCurrentTimestamp() {
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
  ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
  return ss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
  switch (level) {
    case LogLevel::DEBUG:
      return "DEBUG";
    case LogLevel::INFO:
      return "INFO";
    case LogLevel::WARNING:
      return "WARNING";
    case LogLevel::ERROR:
      return "ERROR";
    case LogLevel::FATAL:
      return "FATAL";
    default:
      return "UNKNOWN";
  }
}

void Logger::writeLog(LogLevel level, const std::string& message) {
  if (level < currentLogLevel) {
    return;
  }

  std::lock_guard<std::mutex> lock(logMutex);

  std::string timestamp = getCurrentTimestamp();
  std::string levelStr = logLevelToString(level);
  std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;

  if (consoleOutput) {
    if (level >= LogLevel::ERROR) {
      std::cerr << logMessage << std::endl;
    } else {
      std::cout << logMessage << std::endl;
    }
  }

  if (fileOutput && logFile.is_open()) {
    logFile << logMessage << std::endl;
    logFile.flush();
  }
}

void Logger::debug(const std::string& message) {
  writeLog(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
  writeLog(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
  writeLog(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
  writeLog(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
  writeLog(LogLevel::FATAL, message);
  std::exit(EXIT_FAILURE);
}

std::string Logger::formatString(const std::string& format) {
  return format;
}
