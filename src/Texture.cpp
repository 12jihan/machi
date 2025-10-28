#include "../include/Texture.hpp"
#include "../include/Logger.hpp"

void Texture::setParams() {
  // Set the texture parameters
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set the texture parameters
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::loadTexture(const char* filepath) {
  m_texture_img = Utils::loadImage(filepath);

  if (m_texture_img.data) {
    LOG_INFO("TEXTURE IMAGE DATA LOADING SUCCESS...");

    const int channels = m_texture_img.channels;
    LOG_INFO_F("Image has {} Channels", channels);

    GLenum format;
    switch (channels) {
      case 1:
        format = GL_RED;
        break;
      case 2:
        format = GL_RG;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        LOG_ERROR_F("Issue with channel count: {}", channels);
        format = GL_RGB;  // fallback
        break;
    };

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 m_texture_img.width,
                 m_texture_img.height,
                 0,
                 format,
                 GL_UNSIGNED_BYTE,
                 m_texture_img.data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    LOG_INFO("Possible error with TEXTURE IMAGE DATA ...");
  }
  Utils::freeImage(m_texture_img);
}

Texture::Texture(int texture_slot, const char* filepath) : m_texture_num(texture_slot) {
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  setParams();
  loadTexture(filepath);
};

void Texture::bindTexture() {
  glActiveTexture(GL_TEXTURE0 + m_texture_num);
  LOG_INFO_F("default: {}, After: {}", GL_TEXTURE0, GL_TEXTURE0 + m_texture_num);
  glBindTexture(GL_TEXTURE_2D, m_texture);
}

unsigned const Texture::getTexture() const {
  return m_texture;
};
