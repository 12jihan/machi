#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include <string>
#include <memory>

// Event callback types
using ResizeCallback = std::function<void(int width, int height)>;
using KeyCallback = std::function<void(int key, int scancode, int action, int mods)>;
using MouseButtonCallback = std::function<void(int button, int action, int mods)>;
using MouseMoveCallback = std::function<void(double xpos, double ypos)>;
using ScrollCallback = std::function<void(double xoffset, double yoffset)>;

struct WindowConfig {
  std::string title = "Game Window";
  int width = 1024;
  int height = 768;
  bool resizable = true;
  bool vsync = true;
  bool fullscreen = false;
  int samples = 4;  // MSAA samples

  // OpenGL version
  int glMajorVersion = 3;
  int glMinorVersion = 3;
  bool glCoreProfile = true;
};

class WindowManager {
 private:
  GLFWwindow* window;
  WindowConfig configh;
  bool isInitialized;

  // Current window state
  int currentWidth;
  int currentHeight;
  double lastFrameTime;
  double deltaTime;

  // Event callbacks
  ResizeCallback resizeCallback;
  KeyCallback keyCallback;
  MouseButtonCallback mouseButtonCallback;
  MouseMoveCallback mouseMoveCallback;
  ScrollCallback scrollCallback;

  // Static GLFW callbacks (these call our member functions)
  static void glfwFramebufferSizeCallback(GLFWwindow* window, int width, int height);
  static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
  static void glfwCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
  static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
  static void glfwErrorCallback(int error, const char* description);

  // Helper methods
  void setupCallbacks();
  void setupOpenGLSettings();
  bool initializeGLFW();
  bool createWindow();
  bool initializeOpenGL();

  // void windowHints();
  // void createWindow();
  // void makeContextCur(GLFWwindow* window);

 public:
  WindowManager(const std::string& title = "Machi - Game Engine Default Title", int width = 1024,
                int height = 768);
  ~WindowManager();

  // Delete copy constructor and assignment operator
  // WindowManager(const WindowManager &) = delete;
  // WindowManager &operator=(const WindowManager &) = delete;

  void init();
  void clean();

  bool shouldClose() const;
  void pollEvents() const;
  void swapBuffers() const;
  std::string getTitle();
  const GLFWwindow* getWindow();
  std::array<int, 2> getWindowSize();
};
