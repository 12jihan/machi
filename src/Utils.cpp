#include "../include/Utils.hpp"
#include "../include/Logger.hpp"

// #include "../include/stb_image.h"
#include "../include/stb_image.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <iostream>
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

void loadImage() {
  // Load image, create texture and generate mipmaps
  int width, height, nrChannels;

  unsigned char* data = stbi_load(
    FileSystem::getPath("../textures/wood_texture/wood_texture.png").c_str(), &width, &height, &nrChannels, 0);
}

}  // namespace Utils
