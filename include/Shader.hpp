#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
  unsigned int m_programId;
  bool m_isCompiled;

  unsigned int compileShader(const std::string& src, GLenum shaderType);
  bool linkProgram(unsigned int vertexShader, unsigned int fragmentShader);

public:
  Shader();
  ~Shader();

  // Delete copy operations to prevenet accidental copying
  Shader(const Shader&) = delete;
  Shader& operator=(const Shader&) = delete;

  // Delete copy operations to prevenet accidental copying
  Shader(Shader&& other) noexcept;
  Shader& operator=(Shader&& other) noexcept;

  // Load Shaders from location
  bool loadFromSrc(const std::string& vSrc, const std::string& fSrc);
  bool loadFromFile(const std::string& vPath, const std::string& fPath);

  void use() const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setVec2(const std::string& name, const glm::vec2& value) const;
  void setVec3(const std::string& name, const glm::vec3& value) const;
  void setVec4(const std::string& name, const glm::vec4& value) const;
  void setMat3(const std::string& name, const glm::mat3& value) const;
  void setMat4(const std::string& name, const glm::mat4& value) const;

  unsigned int getId() const {
    return m_programId;
  }
  bool isValid() const {
    return m_isCompiled;
  }
};
