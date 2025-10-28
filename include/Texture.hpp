#pragma once

#include <glad/glad.h>
#include "../include/Utils.hpp"

class Texture {
private:
  unsigned m_texture;
  int m_texture_num = 0;
  Utils::Image m_texture_img;

  void setParams() const;
  void loadTexture(const char* filepath);

public:
  // enum class Format { PNG, JPEG, JPG };
  // enum class Filter { PNG, JPEG, JPG };
  // enum class Wrap { PNG, JPEG, JPG };

  Texture(int texture_slot, const char* filepath);
  void bindTexture();
  unsigned const getTexture() const;
};
