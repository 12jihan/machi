#pragma once

class Renderer {
private:
  // ShaderManager* shaderManager;
  // TextureManager* textureManager;
  // MeshManager* meshManager;
  // Camera* m_ActiveCamera;
  // LightingSystem* lightingSystem;

public:
  Renderer();
  ~Renderer();

  // Renderer lifecycle
  bool intialize();
  void render();
  void setCamera();
  void submit();
};
