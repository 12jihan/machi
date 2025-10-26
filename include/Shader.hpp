#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
public:
  unsigned int m_id;

  // constructor reads and builds the shader
  Shader(const char* vPath, const char* fPath);

  // use/activate the shader
  void use();

  // utility uniform functions
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
  void setMatrix(const std::string& name, glm::mat4& mat) const;
  // void unbind() const;
};
