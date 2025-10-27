#pragma once

#include <glad/glad.h>
#include "../include/Utils.hpp"

enum test {
  "PNG",
  "JPEG",
  "JPG"
}

class Texture {
private:
  unsigned m_texture;
  int m_texture_num = 0;
  Utils::Image m_texture_img;

  void setParams();
  void loadTexture(const char* filepath);

public:
  Texture(int texture_slot, const char* filepath);
  void bindTexture();
  unsigned const getTexture() const;
};
