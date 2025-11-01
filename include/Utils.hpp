#pragma once

#include <string>

namespace Utils {
// Structs
struct Image {
  unsigned char* data = nullptr;

  int width;
  int height;
  int channels;
};

std::string loadFile(const std::string& filepath, bool debug = true);

// unsigned char* loadImage(const std::string& filepath, bool debug = true);

Image loadImage(const std::string& filepath, bool debug = true);

void freeImage(Image& img);

}  // namespace Utils
