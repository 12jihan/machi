#include "../include/InputManager.hpp"

InputManager::InputManager() : m_mouseX(0.0), m_mouseY(0.0), m_scrollX(0.0), m_scrollY(0.0) {
  // Initialize all keys to false (not pressed)
  m_keyStates.fill(false);
  m_mouseButtonStates.fill(false);
}

void InputManager::onEvent(const Event& event) {
  switch (event.type) {
    case EventType::KeyPress:
      break;
    case EventType::KeyRelease:
      break;
    case EventType::MousePress:
      break;
    case EventType::MouseRelease:
      break;
    case EventType::MouseMove:
      break;
    case EventType::MouseScroll:
      break;
    default:
      break;
  }
}

bool InputManager::isKeyPressed(int keycode) const {
  if (keycode >= 0 && keycode < 1024) {
    return m_keyStates[keycode];
  }

  return false;
}

bool InputManager::isMouseButtonPressed(int button) const {
  if (button >= 0 && button < 16) {
    return m_mouseButtonStates[button];
  }

  return false;
}

std::pair<double, double> InputManager::getMousePosition() const {
  return {m_mouseX, m_mouseY};
}
