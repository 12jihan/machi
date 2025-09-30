#include "../include/Shader.hpp"
#include "../include/Logger.hpp"
#include <fstream>
#include <sstream>

Shader::Shader() : m_programId(0), m_isCompiled(false) {}

Shader::~Shader() {
  if (m_programId != 0) {
    glDeleteProgram(m_programId);
    LOG_DEBUG_F("[Shader] Deleted shader program with ID: {}", m_programId);
  }
}

Shader::Shader(Shader&& other) noexcept : m_programId(other.m_programId), m_isCompiled(other.m_isCompiled) {
  other.m_programId = 0;
  other.m_isCompiled = false;
}

Shader& Shader::operator=(Shader&& other) noexcept {
  if (this != &other) {
    if (m_programId) {
      glDeleteProgram(m_programId);
    }

    // Move from other
    m_programId = other.m_programId;
    m_isCompiled = other.m_isCompiled;

    // Reset other
    other.m_programId = 0;
    other.m_isCompiled = false;

    // Reset other
    other.m_programId = 0;
    other.m_isCompiled = false;
  }

  return *this;
}

bool Shader::loadFromSrc(const std::string& vSrc, const std::string& fSrc) {
  LOG_INFO("[Shader] Compiling shader from source...");

  // Compile vertex Shader
  unsigned int vertexShader = compileShader(vSrc, GL_VERTEX_SHADER);
  if (vertexShader == 0) {
    LOG_ERROR("[Shader] Failed to compile vertex shader");
    return false;
  }

  unsigned int fragmentShader = compileShader(fSrc, GL_FRAGMENT_SHADER);
  if (fragmentShader == 0) {
    LOG_ERROR("[Shader] Failed to compile fragment shader");
    glDeleteShader(vertexShader);
    return false;
  }

  // Link program
  bool success = linkProgram(vertexShader, fragmentShader);

  // Clean up shaders
  glDeleteProgram(vertexShader);
  glDeleteProgram(fragmentShader);

  if (success) {
    m_isCompiled = true;
    LOG_INFO_F("[Shader] Shader compiled successfully with programID: {}", m_programId);
  }

  return success;
};

bool Shader::loadFromFile(const std::string& vPath, const std::string& fPath) {
  LOG_INFO_F("[Shader] Loading shader from files: {} and {}", vPath, fPath);

  // Read vertex shader files
  std::ifstream vertexFile(vPath);
  if (!vertexFile.is_open()) {
    LOG_ERROR_F("[Shader] Failed to open vertex shader file: {}", vPath);
    return false;
  }

  std::stringstream vertexStream;
  vertexStream << vertexFile.rdbuf();
  vertexFile.close();

  // Read fragment shader files
  std::ifstream fragmentFile(fPath);
  if (!fragmentFile.is_open()) {
    LOG_ERROR_F("[Shader] Failed to open fragment shader file: {}", fPath);
    return false;
  }

  std::stringstream fragmentStream;
  fragmentStream << fragmentFile.rdbuf();
  fragmentFile.close();

  return loadFromSrc(vertexStream.str(), fragmentStream.str());
};

void Shader::use() const {
  if (m_isCompiled && m_programId != 0) {
    glUseProgram(m_programId);
  } else {
    LOG_WARNING("[Shader] Attempted to use invalid shader program");
  }
}

void Shader::setInt(const std::string& name, int value) const {
  if (m_isCompiled) {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
  }
}

void Shader::setFloat(const std::string& name, float value) const {
  if (m_isCompiled) {
    glUniform1f(glGetUniformLocation(m_programId, name.c_str()), value);
  }
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
  if (m_isCompiled) {
    glUniform2fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
  }
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
  if (m_isCompiled) {
    glUniform3fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
  }
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
  if (m_isCompiled) {
    glUniform4fv(glGetUniformLocation(m_programId, name.c_str()), 1, glm::value_ptr(value));
  }
}

void Shader::setMat3(const std::string& name, const glm::mat3& value) const {
  if (m_isCompiled) {
    glUniformMatrix3fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
  if (m_isCompiled) {
    glUniformMatrix4fv(glGetUniformLocation(m_programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
  }
}

unsigned int Shader::compileShader(const std::string& src, GLenum shaderType) {
  unsigned int shader = glCreateShader(shaderType);
  const char* _src = src.c_str();
  glShaderSource(shader, 1, &_src, nullptr);
  glCompileShader(shader);

  // Check compilation status
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    const char* shaderTypeName = (shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";
    LOG_ERROR_F("[Shader] {} shader compilation failed: {}", shaderTypeName, infoLog);
    glDeleteShader(shader);
    return 0;
  }

  return shader;
}

bool Shader::linkProgram(unsigned int vertexShader, unsigned int fragmentShader) {
  m_programId = glCreateProgram();
  glAttachShader(m_programId, vertexShader);
  glAttachShader(m_programId, fragmentShader);
  glLinkProgram(m_programId);

  // Check linking status
  int success;
  char infoLog[512];
  glGetProgramiv(m_programId, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
    LOG_ERROR_F("[Shader] Shader program linking fialed: {}", infoLog);
    glDeleteProgram(m_programId);
    m_programId = 0;
    return false;
  }

  return true;
}
