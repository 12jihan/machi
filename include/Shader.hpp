#pragma once
#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
private:
public:
  // Shader();
  // ~Shader();

  std::string loadFile(const std::string& filepath);
};
