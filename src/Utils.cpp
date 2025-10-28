#include "../include/Utils.hpp"
#include "stb_image.h"
#include "../include/Logger.hpp"

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

Image loadImage(const std::string& filepath, bool debug) {
  // Image obj
  Image img;
  stbi_set_flip_vertically_on_load(true);

  img.data = stbi_load(filepath.c_str(), &img.width, &img.height, &img.channels, 0);
  LOG_INFO_F("Loading image data: {}", filepath);

  if (img.data) {
    LOG_INFO_F("height: {}, width: {}", img.height, img.width);
    LOG_INFO_F("Number of Channels: {}", img.channels);
  } else {
    LOG_ERROR("Unable to load Image");
  }

  return img;
}

void freeImage(Image& img) {
  if (img.data) {
    stbi_image_free(img.data);
    LOG_INFO("Image data freed");
    return;
  }

  LOG_ERROR("You're attempting to free data that no longer exists...");
}

}  // namespace Utils
