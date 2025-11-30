#pragma once

#include "EventManager.hpp"
#include <array>
// #include <GLFW/glfw3.h>

class InputManager {
private:
  // 1024 array to cover all GLFW key codes
  std::array<bool, 1024> m_keyStates;
  std::array<bool, 16> m_mouseButtonStates;
  double m_mouseX, m_mouseY;
  double m_lastMouseX, m_lastMouseY;
  double m_mouseDeltaX, m_mouseDeltaY;
  bool m_firstMouse;
  double m_scrollX, m_scrollY;

public:
  InputManager();
  ~InputManager() = default;

  // This is the function that connects to the EventManager
  void onEvent(const Event& event);

  // Queries - Basically, used in the game logic
  bool isKeyPressed(int keycode) const;
  bool isMouseButtonPressed(int button) const;
  std::pair<double, double> getMousePosition() const;
  std::pair<double, double> getMouseDelta() const;

  // Optional: for now -- clear scroll every frame since it's an "impulse" not a state
  void clearScroll();
  void resetMouseDelta();
};
