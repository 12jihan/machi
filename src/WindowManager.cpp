#include "../include/WindowManager.hpp"
#include <GLFW/glfw3.h>
#include "../include/Logger.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>

static bool s_glfwInitialized = false;
static int s_windowCount = 0;

WindowManager::WindowManager(const WindowConfig& config) : m_window(nullptr), m_config(config), m_isInitialized(false), m_isFullscreen(false), m_windowedSize{config.width, config.height}, m_windowedPos{100, 100} {}

WindowManager::WindowManager(const std::string& title, int width, int height) : WindowManager(WindowConfig{title, width, height}) {}

WindowManager::~WindowManager() {
  shutdown();
}

bool WindowManager::initialize() {
  if (m_isInitialized) {
    LOG_INFO("[WindowManager] Already initialized");
    return true;
  };

  try {
    if (!s_glfwInitialized) {
      glfwSetErrorCallback(glfwErrorCallback);

      if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
      }

      s_glfwInitialized = true;
      LOG_INFO("[WindowManager] GLFW initialized");
    };

    // Set up window hints
    setupWindowHints();

    // Create window
    GLFWmonitor* monitor = m_config.fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    m_window = glfwCreateWindow(m_config.width, m_config.height, m_config.title.c_str(), monitor, nullptr);

    if (!m_window) {
      throw std::runtime_error("Failed to create GLFW window");
    };

    // Store this instance in GLFW user pointer for callbnacks
    glfwSetWindowUserPointer(m_window, this);

    // Set up callbacks
    setupCallbacks();

    // Make OpenGL context current
    makeContextCurrent();

    // Initialize GLAD
    if (!initializeGLAD()) {
      throw std::runtime_error("Failed to initialize GLAD");
    };

    // Configure initial Setting
    setVSync(m_config.vsync);

    if (!m_config.fullscreen) {
      centerWindow();
    }

    s_windowCount++;
    m_isInitialized = true;

    LOG_INFO_F(
        "[WindowManager] Window created successfully: \n\t+ Title: {} \n\t+(width: {}, "
        "height: "
        "{})",
        m_config.title,
        m_config.width,
        m_config.height);
    return true;
  } catch (const std::exception& e) {
    LOG_INFO_F("[WindowManager] Initialization failed: {}", e.what());
    shutdown();
    return false;
  };
  // windowHints();
  // window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  //
  // // check if window was created
  // if (window == nullptr) {
  //     std::cout << "Trouble Creating GLFW window" << std::endl;
  //     throw std::runtime_error("Trouble Creating GLFW window");
  // }
  //
  // // change the window context
  // glfwMakeContextCurrent(window);
  // // TODO: add this in when you get to user input...
  // //  glfwSetFramebufferSizeCallback(window, frameBufferCB);
  //
  // if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
  //     std::cout << "Failed to initialize GLAD" << std::endl;
  //     glfwTerminate();
  //     throw std::runtime_error("Trouble Setting Up Glad");
  // }
  //
  // glfwMakeContextCurrent(window);
}

void WindowManager::setupWindowHints() {
  // OpenGL version and profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_config.glMajorVersion);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_config.glMinorVersion);

  if (m_config.glCoreProfile) {
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }

  // macOS compatibility
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

  // Window properties
  glfwWindowHint(GLFW_RESIZABLE, m_config.resizable ? GLFW_TRUE : GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, m_config.decorated ? GLFW_TRUE : GLFW_FALSE);

  // Multisampling (MSAA)
  if (m_config.samples > 0) {
    glfwWindowHint(GLFW_SAMPLES, m_config.samples);
  }

  // Double buffering (always enabled for games)
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
}

void WindowManager::setupCallbacks() {
  glfwSetFramebufferSizeCallback(m_window, glfwResizeCallback);
  glfwSetKeyCallback(m_window, glfwKeyCallback);
  glfwSetMouseButtonCallback(m_window, glfwMouseButtonCallback);
  glfwSetCursorPosCallback(m_window, glfwMouseMoveCallback);
  glfwSetScrollCallback(m_window, glfwScrollCallback);
}

bool WindowManager::initializeGLAD() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    return false;
  }

  LOG_INFO_F("[WindowManager] OpenGL: {}", glGetString(GL_VERSION));
  LOG_INFO_F("[WindowManager] GPU: {}", glGetString(GL_RENDERER));

  return true;
}

void WindowManager::centerWindow() {
  if (!m_window)
    return;
  auto [monitorWidth, monitorHeight] = getPrimaryMonitorWorkArea();
  int x = (monitorWidth - m_config.width) / 2;
  int y = (monitorWidth - m_config.width) / 2;

  glfwSetWindowPos(m_window, x, y);
}

// Static GLFW callbacks
void WindowManager::glfwErrorCallback(int error, const char* description) {
  LOG_ERROR_F("[GLFW Error {}] {}", error, description);
}

void WindowManager::glfwResizeCallback(GLFWwindow* window, int width, int height) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  if (windowManager) {
    // Update config
    windowManager->m_config.width = width;
    windowManager->m_config.height = height;

    // Call user callback
    if (windowManager->m_resizeCallback) {
      windowManager->m_resizeCallback(width, height);
    }

    // Update viewport by default
    glViewport(0, 0, width, height);
  }
}

void WindowManager::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  if (windowManager && windowManager->m_keyCallback) {
    windowManager->m_keyCallback(key, scancode, action, mods);
  }
}

void WindowManager::glfwMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  if (windowManager && windowManager->m_mouseButtonCallback) {
    windowManager->m_mouseButtonCallback(button, action, mods);
  }
}

void WindowManager::glfwMouseMoveCallback(GLFWwindow* window, double xpos, double ypos) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  if (windowManager && windowManager->m_mouseMoveCallback) {
    windowManager->m_mouseMoveCallback(xpos, ypos);
  }
}

void WindowManager::glfwScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  auto* windowManager = static_cast<WindowManager*>(glfwGetWindowUserPointer(window));
  if (windowManager && windowManager->m_scrollCallback) {
    windowManager->m_scrollCallback(xoffset, yoffset);
  }
}

void WindowManager::shutdown() {
  if (m_window) {
    glfwDestroyWindow(m_window);
    m_window = nullptr;
    s_windowCount--;
  }

  // Terminate GLFW if this was the last window
  if (s_windowCount == 0 && s_glfwInitialized) {
    glfwTerminate();
    s_glfwInitialized = false;
    LOG_INFO("[WindowManager] GLFW terminated");
  }

  m_isInitialized = false;
}

bool WindowManager::shouldClose() const {
  return m_window ? glfwWindowShouldClose(m_window) : true;
}

void WindowManager::swapBuffers() const {
  if (m_window) {
    glfwSwapBuffers(m_window);
  }
}

void WindowManager::pollEvents() const {
  glfwPollEvents();
}

void WindowManager::close() {
  if (m_window) {
    glfwSetWindowShouldClose(m_window, GLFW_TRUE);
  }
}

std::string WindowManager::getTitle() const {
  return m_config.title;
}

void WindowManager::setTitle(const std::string& title) {
  m_config.title = title;
  if (m_window) {
    glfwSetWindowTitle(m_window, title.c_str());
  }
}

std::array<int, 2> WindowManager::getSize() const {
  if (m_window) {
    int width, height;
    glfwGetWindowSize(m_window, &width, &height);
    return {width, height};
  }
  return {m_config.width, m_config.height};
}

void WindowManager::setSize(int width, int height) {
  m_config.width = width;
  m_config.height = height;

  if (m_window) {
    glfwSetWindowSize(m_window, width, height);
  }
}

std::array<int, 2> WindowManager::getFramebufferSize() const {
  if (m_window) {
    int width, height;
    glfwGetFramebufferSize(m_window, &width, &height);
    return {width, height};
  }

  return getSize();
}

void WindowManager::setFullscreen(bool fullscreen) {
  if (!m_window || m_isFullscreen == fullscreen) {
    return;
  }

  if (fullscreen) {
    // Store current windowed state
    auto [width, height] = getSize();
    auto [x, y] = getPosition();
    m_windowedSize = {width, height};
    m_windowedPos = {x, y};

    // Get primary monitor
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Set fullscreen
    glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
  } else {
    // Restore windowed mode
    glfwSetWindowMonitor(m_window, nullptr, m_windowedPos[0], m_windowedPos[1], m_windowedSize[0], m_windowedSize[1], 0);
  }

  m_isFullscreen = fullscreen;
}

void WindowManager::toggleFullscreen() {
  setFullscreen(!m_isFullscreen);
}

bool WindowManager::getVSync() const {
  return m_config.vsync;
}

void WindowManager::setVSync(bool enabled) {
  m_config.vsync = enabled;
  glfwSwapInterval(enabled ? 1 : 0);
}

bool WindowManager::isKeyPressed(int key) const {
  return m_window ? glfwGetKey(m_window, key) == GLFW_PRESS : false;
}

bool WindowManager::isMouseButtonPressed(int button) const {
  return m_window ? glfwGetMouseButton(m_window, button) == GLFW_PRESS : false;
}

std::array<double, 2> WindowManager::getMousePosition() const {
  if (m_window) {
    double x, y;
    glfwGetCursorPos(m_window, &x, &y);
    return {x, y};
  }

  return {0.0, 0.0};
}

void WindowManager::makeContextCurrent() {
  if (m_window) {
    glfwMakeContextCurrent(m_window);
  }
}

std::array<int, 2> WindowManager::getPrimaryMonitorSize() const {
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  const GLFWvidmode* mode = glfwGetVideoMode(monitor);
  return {mode->width, mode->height};
}
