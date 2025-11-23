#include "../include/Engine.hpp"
#include "../include/Logger.hpp"
#include "../include/Shader.hpp"
// #include "../include/Utils.hpp"
#include "../include/Texture.hpp"

#include <GLFW/glfw3.h>

#include <chrono>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

Engine::Engine(const EngineConfig& config) :
 m_config(config),
 m_isInitialized(false),
 m_isRunning(false),
 m_isPaused(false),
 m_deltaTime(0.0f),
 m_lastFrame(0.0f),
 m_currentFrame(0.0f),
 m_totalTime(0.0f),
 m_frameCount(0.0f),
 m_fps(0.0f),
 m_fpsUpdateTimer(0.0f),
 m_windowManager(nullptr)
// m_currentScene(nullptr),
// m_nextScene(nullptr)
{
  if (m_config.enableLogging) {
    Logger::getInstance().enableFileOutput(true, m_config.logFile);
    Logger::getInstance().setLogLevel(LogLevel::INFO);
  }

  LOG_INFO_F("[Engine] Engine created with title: '{}', size: {}x{}",
             m_config.windowTitle,
             m_config.windowWidth,
             m_config.windowHeight);
}

Engine::Engine(const std::string& title, int width, int height) : Engine(EngineConfig{title, width, height}) {
  // This delegates to the main constructor with a default config.
  LOG_INFO_F("[Engine] Engine created with title: '{}', size: {}x{}",
             m_config.windowTitle,
             m_config.windowWidth,
             m_config.windowHeight);
}

Engine::~Engine() {
  LOG_INFO("[Engine] Engine shuttingdown");
  shutdown();
}

bool Engine::initialize() {
  if (m_isInitialized) {
    LOG_INFO("[Engine] Engine already intialized!");
    return true;
  }

  LOG_INFO("[Engine] Beginning engine initialization...");
  m_engineStartTime = std::chrono::high_resolution_clock::now();

  try {
    // Initialize all engine subsystems in the correct order
    // Window system must come first as other systems depend on OpengGL context
    if (!initializeWindowSystem()) {
      LOG_ERROR("[Engine] Failed to initialize window system!");
      return false;
    }
    // TODO: Create Render class
    // if (!initializeRenderingSystem()) {
    //   LOG_ERROR("[Engine] Failed to initialize rendering system!");
    //   return false;
    // }
    if (!initializeInputSystem()) {
      LOG_ERROR("[Engine] Failed to initialize input system!");
      return false;
    }
    // Audio system is optional for now
    // initializeAudioSystem();

    // Set up intial timing
    m_lastFrameTime = std::chrono::high_resolution_clock::now();

    m_isInitialized = true;
    LOG_INFO("[Engine] Engine intialization completed sucessfully!");

    // Print useful information about the system
    printSystemInfo();

    return true;

  } catch (const std::exception& e) {
    LOG_ERROR_F("[Engine] Initization failed with exception: {}", e.what());
    shutdownSystems();
    return false;
  }
}

bool Engine::initializeWindowSystem() {
  LOG_INFO("[Engine] Initializing window system");
  // Create window configuration from our engine config
  WindowConfig windowConfig;
  windowConfig.title = m_config.windowTitle;
  windowConfig.width = m_config.windowWidth;
  windowConfig.height = m_config.windowHeight;
  windowConfig.fullscreen = m_config.fullscreen;
  windowConfig.vsync = m_config.vsync;
  windowConfig.samples = m_config.msaaSamples;

  m_windowManager = std::make_unique<WindowManager>(windowConfig);

  if (!m_windowManager->initialize()) {
    LOG_ERROR("[Engine] Failed to initialize WindowManager");
    return false;
  }

  // Setup all the window event callbacks to bridge into our event system
  m_windowManager->setResizeCallback([this](int width, int height) -> void { onWindowResize(width, height); });

  m_windowManager->setKeyCallback(
    [this](int key, int scancode, int actions, int mods) -> void { onKeyEvent(key, scancode, actions, mods); });

  m_windowManager->setMouseButtonCallback(
    [this](int button, int action, int mods) -> void { onMouseButton(button, action, mods); });

  m_windowManager->setScrollCallback([this](double xOffset, double yOffset) { onScroll(xOffset, yOffset); });

  // m_windowManager->setMouseMoveCallback([this](double x, double y) -> void { onMouseMove(x, y); });

  LOG_INFO("[Engine] Window system intialized sucessfully");
  return true;
};

// TODO: extend the renderer class
bool Engine::initializeRenderingSystem() {
  LOG_INFO("[Engine] Initializing rendering system...");
  // Set up basic OpenGL state based on our configuration
  setClearColor(m_config.clearR, m_config.clearG, m_config.clearB, m_config.clearA);
  enableDepthTest(m_config.enableDepthTest);
  enableBlending(m_config.enableBlending);

  // Set initial viewport to match window size
  auto [width, height] = m_windowManager->getSize();
  glViewport(0, 0, width, height);

  LOG_INFO("[Engine] Rendering system initialized successfully");
  return true;
}

bool Engine::initializeInputSystem() {
  LOG_INFO("[Engine] Initializing input system...");

  addEventListener([this](const EngineEvent& event) {
    if (event.type == EngineEventType::KeyPress && m_isRunning) {
      switch (event.data.keyboard.key) {
        case GLFW_KEY_ESCAPE:
          LOG_INFO("[Engine] Escape pressed - requesting shutdown");
          shutdown();
          break;
        case GLFW_KEY_F2:
          LOG_INFO("[Engine] F2 pressed - toggling fullscreen");
          m_windowManager->toggleFullscreen();
          break;
        case GLFW_KEY_F1:
          printFrameStats();
          break;
      }
    }
  });

  addEventListener([this](const EngineEvent& event) {
    if (event.type == EngineEventType::MousePress && m_isRunning) {
      switch (event.data.mouse.button) {
        case GLFW_MOUSE_BUTTON_1:
          LOG_INFO("[Engine] Mouse Button 1 Pressed.");
          break;
        case GLFW_MOUSE_BUTTON_2:
          LOG_INFO("[Engine] Mouse Button 2 Pressed.");
          break;
        case GLFW_MOUSE_BUTTON_3:
          LOG_INFO("[Engine] Mouse Button 3 Pressed.");
          break;
        case GLFW_MOUSE_BUTTON_4:
          LOG_INFO("[Engine] Mouse Button 4 Pressed.");
          break;
        case GLFW_MOUSE_BUTTON_5:
          LOG_INFO("[Engine] Mouse Button 5 Pressed.");
          break;
        case GLFW_MOUSE_BUTTON_6:
          LOG_INFO("[Engine] Mouse Button 6 Pressed.");
          break;
      }
    }
  });

  addEventListener([this](const EngineEvent& event) {
    if (event.type == EngineEventType::MouseScroll && m_isRunning) {
      float scrollAmount = static_cast<float>(event.data.scroll.yOffset);
      LOG_INFO_F("[Engine] Scroll amount: {}", scrollAmount);
    }
  });

  addEventListener([this](const EngineEvent& event) -> void {
    if (event.type == EngineEventType::KeyPress && m_isRunning) {
      switch (event.data.keyboard.key) {
        case GLFW_KEY_W:
          LOG_INFO_F("Key Pressed: {}", "W");
          break;
        case GLFW_KEY_A:
          LOG_INFO_F("Key Pressed: {}", "A");
          break;
        case GLFW_KEY_S:
          LOG_INFO_F("Key Pressed: {}", "S");
          break;
        case GLFW_KEY_D:
          LOG_INFO_F("Key Pressed: {}", "D");
          break;
        case GLFW_KEY_F:
          LOG_INFO_F("Key Pressed: {}", "F");
          break;
        case GLFW_KEY_E:
          LOG_INFO_F("Key Pressed: {}", "E");
          break;
        case GLFW_KEY_Q:
          LOG_INFO_F("Key Pressed: {}", "Q");
          break;
      }
    }
  });

  LOG_INFO("[Engine] Input system intialized successfully");
  return true;
}

// bool Engine::intializeAudioSystem() {
//   LOG_INFO("[Engine] Audio system intialization skipped (not implemented yet)");
//   return true;
// }

void Engine::run() {
  if (!m_isInitialized) {
    LOG_INFO("[Engine] Cannot run - engine not intialized!");
    return;
  }

  glEnable(GL_DEPTH_TEST);

  // INFO: --> Shader test starts here
  Shader shader("../resources/shaders/main.vert.glsl", "../resources/shaders/main.frag.glsl");

  // VAOs, VBOs, EBOs
  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),
    glm::vec3(-1.3f, 1.0f, -1.5f)
  };

  unsigned int indices[] = {  
    0, 1, 3,
    1, 2, 3
  };
  // clang-format on

  unsigned int vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // vao
  glBindVertexArray(vao);

  // vbo
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // ebo
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // texture attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Load and create texture
  Texture texture0(0, "../resources/textures/wood_texture/wood_texture.png");
  Texture texture1(1, "../resources/textures/awesomeface.png");

  // Use Shader
  shader.use();
  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);

  // INFO: --> Shader test ends here
  LOG_INFO("[ENGINE] starting main engine loop...");
  m_isRunning = true;
  m_lastFrameTime = std::chrono::high_resolution_clock::now();

  // Main engine, loop - this is the heart of your game engine
  LOG_INFO_F("[Engine]::[Shader] m_isPaused {}", m_isPaused);

  LOG_INFO_F("checking the window config frame: {} x {}", m_config.windowWidth, m_config.windowHeight);
  auto start_time = std::chrono::high_resolution_clock::now();

  glm::mat4 projection =
    glm::perspective(glm::radians(45.0f), (float)m_config.windowWidth / (float)m_config.windowHeight, 0.1f, 100.0f);
  shader.setMat4("projection", projection);
  float yaw = -90.0f;
  float pitch = -90.0f;

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));

  while (m_isRunning && !m_windowManager->shouldClose()) {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    m_currentFrame = std::chrono::duration<float>(now).count();
    m_deltaTime = m_currentFrame - m_lastFrame;
    m_lastFrame = m_currentFrame;

    processEvents();
    if (!m_isRunning)
      break;
    // keyTest(m_windowManager->getWindow());

    glClearColor(0.1, 0.0, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // bind Texture
    texture0.bindTexture();
    texture1.bindTexture();

    // Activate Shader & Create transformations
    shader.use();
    //
    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    // glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::vec3 cameraUp = glm::normalize(cameraPos - cameraTarget);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::normalize(glm::cross(cameraDirection, cameraRight));

    glm::mat4 view = glm::mat4(1.0f);
    float radius = 10.0f;
    float camX = static_cast<float>(sin(m_deltaTime) * radius);
    float camZ = static_cast<float>(cos(m_deltaTime) * radius);
    // view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.setMat4("view", view);

    glBindVertexArray(vao);
    for (unsigned int i = 0; i < 10; i++) {
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      // model = glm::rotate(model, dt_seconds * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.setMat4("model", model);

      glDrawArrays(GL_TRIANGLES, 0, 36);
    };

    m_windowManager->swapBuffers();

    // Update frame statistics for performance monitoring
    calculateFrameStats();
    // }
    // TODO: Create Scene class
    // Handle scene transitions if needed
    // performSceneTransition();
  }

  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);

  LOG_INFO("[Engine] Main engine loop ended");
}

void Engine::processEvents() {
  // Poll for new window events from GLFW
  m_windowManager->pollEvents();

  // Process our internal event queue
  processEventQueue();
}

void Engine::updateSystems(float deltaTime) {
  // TODO: implement Scene class
  // Update the current scene if we have one
  // if (m_currentScene) {
  //   m_currentScene->update(deltaTime);
  // }

  // Here you would update other engine system like:
  // - Physics system
  // - Audio system
  // - Animation system
  // - Particle systems
  // - UI system
};

void Engine::renderFrame() {
  LOG_INFO("[Engine] running renderer");
  // Clear the screen with our configured background color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO: Implement Scene class
  // Render the current scene if we have one
  // if (m_currentScene) {
  //   m_currentScene->render();
  // }

  // Preset the rendered frame to the screen
  m_windowManager->swapBuffers();
}

void Engine::calculateFrameStats() {
  m_frameCount++;
  m_fpsUpdateTimer += m_deltaTime;

  // Update FPS display every half second
  if (m_fpsUpdateTimer >= 0.5f) {
    m_fps = m_frameCount / m_fpsUpdateTimer;
    m_frameCount = 0;
    m_fpsUpdateTimer = 0.0f;

    // Update window title with FPS if requested
    if (m_config.showFPSInTitle) {
      updateWindowTitle();
    }
  }
}

void Engine::updateWindowTitle() {
  std::ostringstream title;
  title << m_config.windowTitle << " - FPS: " << std::fixed << std::setprecision(1) << m_fps;
  m_windowManager->setTitle(title.str());
}

// Window event handlers - these translate WindowManager callbacks into our event system
void Engine::onWindowResize(int width, int height) {
  // Update our configuration
  m_config.windowWidth = width;
  m_config.windowHeight = height;

  // Update OpenGL viewport
  glViewport(0, 0, width, height);

  // Create and dispatch resize event
  EngineEvent event;
  event.type = EngineEventType::WindowResize;
  event.data.resize = {width, height};
  event.timestamp = m_totalTime;

  dispatchEvent(event);

  LOG_INFO_F("[Event] Window resized to {}x{}", width, height);
}

void Engine::onKeyEvent(int key, int scancode, int action, int mods) {
  LOG_DEBUG_F("Key Pressed: {}, {}", key, action);
  EngineEvent event;
  event.type =
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? EngineEventType::KeyPress : EngineEventType::KeyRelease;
  event.data.keyboard = {key, scancode, mods};
  event.timestamp = m_totalTime;
  dispatchEvent(event);
}

void Engine::onMouseButton(int button, int action, int mods) {
  LOG_DEBUG_F("Mouse Button Pressed: {}", button);

  EngineEvent event;
  event.type =
    (action == GLFW_PRESS || action == GLFW_REPEAT) ? EngineEventType::MousePress : EngineEventType::MouseRelease;
  event.data.mouse = {button, mods};
  event.timestamp = m_totalTime;
  dispatchEvent(event);
}

void Engine::keyTest(GLFWwindow* window) {
  const float cameraSpeed = 0.05f;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
};

void Engine::onMouseMove(double x, double y) {
  EngineEvent event;
  event.type = EngineEventType::MouseMove;
  event.data.mousePos = {x, y};
  event.timestamp = m_totalTime;
  LOG_DEBUG_F("Mouse Movement: {}, {}", x, y);
  dispatchEvent(event);
}

void Engine::onScroll(double xOffset, double yOffset) {
  // Could be extended to log scrolls
  EngineEvent event;
  event.type = EngineEventType::MouseScroll;
  event.data.scroll = {xOffset, yOffset};
  event.timestamp = m_totalTime;

  dispatchEvent(event);

  LOG_DEBUG_F("[Engine] Mouse scroll: ({}, {})", xOffset, yOffset);
}

void Engine::dispatchEvent(const EngineEvent& event) {
  // Add event to queue for processing
  m_eventQueue.push_back(event);
}

void Engine::processEventQueue() {
  // Process all event in the queue
  for (const auto& event : m_eventQueue) {
    // Send event to all registered handlers
    for (const auto& handler : m_eventHandlers) {
      handler(event);
    }
  }

  if (!m_eventQueue.empty()) {
    m_eventQueue.clear();
  };
}

// TODO: Implement Scene class
// void Engine::performSceneTransition() {
//   if (m_nextScene) {
//     // Clean up currentScene
//     if (m_currentScene) {
//       m_currentScene.reset();
//     }
//
//     // Switch to new scene
//     mov_currentScene = std::move(m_nextScene);
//     m_nextScene = nullptr;
//
//     LOG_INFO("[Engine] Scene transition completed");
//   }
// }

// TODO: Implement Scene class
// Public interface methods
// Engine::setScene(std::unique_ptr<Scene> scene) {
//   m_currentScene = std::move(scene);
//   Log_INFO("[Engine] Scene set directly");
// }

// TODO: Implement Scene class
// void Engine::transitionToScene(std::unique_ptr<Scene> scene) {
//   m_nextScene = std::move(scene);
//   LOG_INFO("[Engine] Scene transition queue");
// }

void Engine::addEventListener(const EventHandler& handler) {
  m_eventHandlers.push_back(handler);
}

std::array<int, 2> Engine::getWindowSize() const {
  return m_windowManager ? m_windowManager->getSize() : std::array<int, 2>{0, 0};
}

void Engine::setWindowSize(int width, int height) {
  EngineEvent resizeEvent;
  resizeEvent.type = EngineEventType::WindowResize;

  if (m_windowManager) {
    m_windowManager->setSize(width, height);
    m_config.windowWidth = width;
    m_config.windowHeight = height;
  }
}

void Engine::setWindowTitle(const std::string& title) {
  m_config.windowTitle = title;
  if (m_windowManager) {
    m_windowManager->setTitle(title);
  }
}

// TODO: Fix fullscreen
void Engine::toggleFullscreen() {
  if (m_windowManager) {
    m_windowManager->toggleFullscreen();
    m_config.fullscreen = m_windowManager->isFullscreen();
  }
}

bool Engine::isFullscreen() const {
  return m_windowManager ? m_windowManager->isFullscreen() : false;
}

void Engine::setClearColor(float r, float g, float b, float a) {
  m_config.clearR = r;
  m_config.clearG = g;
  m_config.clearB = b;
  m_config.clearA = a;
  glClearColor(r, g, b, a);
}

void Engine::enableDepthTest(bool enable) {
  m_config.enableDepthTest = enable;
  if (enable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  };
}

void Engine::enableBlending(bool enable) {
  m_config.enableDepthTest = enable;
  if (enable) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else {
    glDisable(GL_BLEND);
  };
}

void Engine::enableVSync(bool enable) {
  m_config.vsync = enable;
  if (m_windowManager) {
    m_windowManager->setVSync(enable);
  };
}

void Engine::pause() {
  m_isPaused = true;
  LOG_INFO("[Engine] Engine paused");
}

void Engine::resume() {
  m_isPaused = false;
  // Reset frame timeing to avoid large delta time jump
  m_lastFrameTime = std::chrono::high_resolution_clock::now();
  LOG_INFO("[Engine] Engine resumed");
}

void Engine::shutdown() {
  if (!m_isInitialized && !m_isRunning) {
    return;
  }

  LOG_INFO("[Engine] Shutting down engine...");
  m_isRunning = false;

  // Dispatch shutdown event
  EngineEvent shutdownEvent;
  shutdownEvent.type = EngineEventType::EngineShutdown;
  shutdownEvent.timestamp = m_totalTime;
  dispatchEvent(shutdownEvent);
  processEventQueue();

  // shutdownSystems();

  m_isInitialized = false;
  LOG_INFO("[Engine] Engine shutdown completed");
}

void Engine::shutdownSystems() {
  // TODO: Create Scene class
  // Clean up scenes
  // m_currentScene.reset();
  // m_nextScene.reset();

  // Clear event handlers
  m_eventHandlers.clear();
  m_eventQueue.clear();

  // WindowManager will clean up automatically through its destructor
  m_windowManager.reset();
}

void Engine::printSystemInfo() const {
  LOG_INFO("=== ENGINE SYSTEM INFORMATION ===");
  LOG_INFO_F("Engine Version: {}", getEngineVersion());
  LOG_INFO_F("Window Size: {}x{}", m_config.windowWidth, m_config.windowHeight);
  LOG_INFO_F("Fullscreen: {}", m_config.fullscreen ? "Yes" : "No");
  LOG_INFO_F("VSync: {}", m_config.vsync ? "Enabled" : "Disabled");
  LOG_INFO_F("MSAA Samples: {}", m_config.msaaSamples);

  if (m_windowManager) {
    LOG_INFO_F("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
    LOG_INFO_F("Graphics Card: {}", (const char*)glGetString(GL_RENDERER));
    LOG_INFO_F("OpenGL Vendor: {}", (const char*)glGetString(GL_VENDOR));
  }

  LOG_INFO("================================");
}

void Engine::printFrameStats() const {
  LOG_INFO("=== FRAME STATISTICS ===");
  LOG_INFO_F("Current FPS: {:.1f}", m_fps);
  LOG_INFO_F("Frame Time: {:.3f}ms", m_deltaTime * 1000.0f);
  LOG_INFO_F("Total Runtime: {:.2f}s", m_totalTime);
  LOG_INFO("========================");
}

std::string Engine::getBuildInfo() {
  std::ostringstream info;
  info << "Built on " << __DATE__ << " at " << __TIME__;
  info << " | OpenGL " << (const char*)glGetString(GL_VERSION);
  return info.str();
}
