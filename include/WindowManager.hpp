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
  bool fullscreen = false;
  bool resizable = true;
  bool vsync = true;
  bool decorated = true;  // Window border/title bar
  int samples = 4;        // MSAA samples (0 = disabled)

  // OpenGL version
  int glMajorVersion = 3;
  int glMinorVersion = 3;
  bool glCoreProfile = true;
};

class WindowManager {
private:
  // Core window data
  GLFWwindow* m_window;
  WindowConfig m_config;
  bool m_isInitialized;

  // Window state
  int m_isFullscreen;
  std::array<int, 2> m_windowedSize;  // Store windowed size when going fullscreen
  std::array<int, 2> m_windowedPos;   // Store windowed position

  // Callbacks
  ResizeCallback m_resizeCallback;
  KeyCallback m_keyCallback;
  MouseButtonCallback m_mouseButtonCallback;
  MouseMoveCallback m_mouseMoveCallback;
  ScrollCallback m_scrollCallback;

  // Static GLFW callbacks (these call our member functions)
  static void glfwErrorCallback(int error, const char* description);
  static void glfwResizeCallback(GLFWwindow* window, int width, int height);
  static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
  static void glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos);
  static void glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

  // Helper methods
  void setupWindowHints();
  void setupCallbacks();
  bool initializeGLAD();
  void centerWindow();

public:
  // Constructor/Destructor
  explicit WindowManager(const WindowConfig& config = {});
  WindowManager(const std::string& title, int width, int height);
  ~WindowManager();

  // Delete copy constructor and assignment operator
  WindowManager(const WindowManager&) = delete;
  WindowManager& operator=(const WindowManager&) = delete;

  // Core window operations
  bool initialize();
  void shutdown();
  bool shouldClose() const;
  void swapBuffers() const;
  void pollEvents() const;
  void close();

  // Window properties
  std::string getTitle() const;
  void setTitle(const std::string& title);

  std::array<int, 2> getSize() const;
  void setSize(int width, int height);

  std::array<int, 2> getPosition() const;
  void setPosition();

  std::array<int, 2> getFramebufferSize() const;

  // Fullscreen management
  bool isFullscreen() const {
    return m_isFullscreen;
  };

  void setFullscreen(bool fullscreen);
  void toggleFullscreen();

  // VSync
  bool getVSync() const;
  void setVSync(bool enabled);

  // Visibility
  void show();
  void hide();
  void minimize();
  void maximize();
  void restore();

  // Input state queries (for polling)
  bool isKeyPressed(int key) const;
  bool isMouseButtonPressed(int button) const;
  std::array<double, 2> getMousePosition() const;

  // Callback setters
  void setResizeCallback(const ResizeCallback& callback) {
    m_resizeCallback = callback;
  };
  void setKeyCallback(const KeyCallback& callback) {
    m_keyCallback = callback;
  };
  void setMouseButtonCallback(const MouseButtonCallback& callback) {
    m_mouseButtonCallback = callback;
  };
  void setMouseMoveback(const MouseMoveCallback& callback) {
    m_mouseMoveCallback = callback;
  };
  void setScrollCallback(const ScrollCallback& callback) {
    m_scrollCallback = callback;
  };

  // Clear callbacks
  void clearResizeCallback(const ResizeCallback& callback) {
    m_resizeCallback = nullptr;
  };
  void clearKeyCallback(const KeyCallback& callback) {
    m_keyCallback = nullptr;
  };
  void clearMouseButtonCallback(const MouseButtonCallback& callback) {
    m_mouseButtonCallback = nullptr;
  };
  void clearMouseMoveback(const MouseMoveCallback& callback) {
    m_mouseMoveCallback = nullptr;
  };
  void clearScrollCallback(const ScrollCallback& callback) {
    m_scrollCallback = nullptr;
  };

  // Getters
  GLFWwindow* getWindow() const {
    return m_window;
  };
  const WindowConfig& getConfig() const {
    return m_config;
  };
  bool isInitialized() const {
    return m_isInitialized;
  }

  // Utility
  void makeContextCurrent();
  void setClipboardString(const std::string& text);
  std::string getClipboardString() const;

  // Monitor/Display info
  std::array<int, 2> getPrimaryMonitorSize() const;
  std::array<int, 2> getPrimaryMonitorWorkArea() const;
};
