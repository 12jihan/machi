#include "../include/Shader.hpp"
#include "../include/Utils.hpp"
#include "../include/Logger.hpp"

Shader::Shader(const char* vPath, const char* fPath) {
  std::string vertexCode = Utils::loadFile(vPath);
  std::string fragmentCode = Utils::loadFile(fPath);

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  unsigned vShader, fShader;
  int success;
  char infoLog[512];

  // Vertex Shader
  vShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vShader, 1, &vShaderCode, nullptr);
  glCompileShader(vShader);
  glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vShader, 512, nullptr, infoLog);
    LOG_ERROR_F("[Shader] there was an error with the vShader: {}", infoLog);
  };

  // Vertex Shader
  fShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fShader, 1, &fShaderCode, nullptr);
  glCompileShader(fShader);
  glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fShader, 512, nullptr, infoLog);
    LOG_ERROR_F("[Shader] there was an error with the fShader: {}", infoLog);
  };

  // Shader Program
  m_id = glCreateProgram();
  glAttachShader(m_id, vShader);
  glAttachShader(m_id, fShader);
  glLinkProgram(m_id);
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
  };

  // Delete now that they've been binded to the shader program
  glDeleteShader(vShader);
  glDeleteShader(fShader);
}

void Shader::use() {
  glUseProgram(m_id);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
  glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

// void unbind() const {}
