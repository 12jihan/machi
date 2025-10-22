#include <string>

namespace Utils {
// Structs
struct Image {
  unsigned char* data = nullptr;

  unsigned int width;
  unsigned int height;
  unsigned int channels;
};

std::string loadFile(const std::string& filepath, bool debug = true);

Image loadImageTest(const std::string& filepath, bool debug = true);

void freeImag(Image& img);

}  // namespace Utils
