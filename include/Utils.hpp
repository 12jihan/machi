#include "../include/Logger.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>

class Utils {
public:
  std::string loadFile(const std::string& filepath) {
    std::ifstream file(filepath);

    if (!file.is_open()) {
      LOG_ERROR_F("\n[Shader] Problem opening shader file: {}", filepath);
      throw std::runtime_error("There was an error loading the file");
    }
    LOG_INFO_F("Reading Shader File: {}", filepath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
  }
};
