#include "../include/Utils.hpp"
#include "../include/Logger.hpp"

#include <fstream>
#include <stdexcept>
#include <sstream>

namespace Utils {
std::string loadFile(const std::string& filepath, bool debug) {
  std::ifstream file(filepath);

  if (!file.is_open()) {
    LOG_ERROR_F("\nProblem opening file: {}", filepath);
    throw std::runtime_error("There was an error loading the file");
  }

  if (debug)
    LOG_INFO_F("Reading File: {}", filepath);

  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();

  if (debug)
    LOG_INFO_F("Done Reading File: {}", filepath);

  return buffer.str();
}

}  // namespace Utils
