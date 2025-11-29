#include "../include/InputManager.hpp"
#include "../include/Logger.hpp"

InputManager::InputManager() : m_mouseX(0.0), m_mouseY(0.0), m_scrollX(0.0), m_scrollY(0.0) {
  // Initialize all keys to false (not pressed)
  m_keyStates.fill(false);
  m_mouseButtonStates.fill(false);
}

void InputManager::onEvent(const Event& event) {
  switch (event.type) {
    case EventType::KeyPress:
      // Prevent out of bounds access
      if (event.data.keyboard.key >= 0 && event.data.keyboard.key < 1024) {
        m_keyStates[event.data.keyboard.key] = true;
        LOG_INFO_F("Key Pressed: {}", event.data.keyboard.key);
      }
      break;

    case EventType::KeyRelease:
      // Prevent out of bounds access
      if (event.data.keyboard.key >= 0 && event.data.keyboard.key < 1024) {
        m_keyStates[event.data.keyboard.key] = false;
        LOG_INFO_F("Key Released: {}", event.data.keyboard.key);
      }
      break;

    case EventType::MousePress:
      if (event.data.keyboard.key >= 0 && event.data.keyboard.key < 16) {
        m_mouseButtonStates[event.data.mouse.button] = true;
        LOG_INFO_F("Key Pressed: {}", event.data.mouse.button);
      }
      break;

    case EventType::MouseRelease:
      if (event.data.keyboard.key >= 0 && event.data.keyboard.key < 16) {
        m_mouseButtonStates[event.data.mouse.button] = false;
        LOG_INFO_F("Mouse Released: {}", event.data.mouse.button);
      }
      break;

    case EventType::MouseMove:
      m_mouseX = event.data.mousePos.x;
      m_mouseY = event.data.mousePos.y;
      LOG_INFO_F("Mouse Position: ({}, {})", event.data.mousePos.x, event.data.mousePos.y);
      break;

    case EventType::MouseScroll:
      m_mouseX = event.data.scroll.xOffset;
      m_mouseY = event.data.scroll.yOffset;
      LOG_INFO_F("Mouse Position: ({}, {})", event.data.mousePos.x, event.data.mousePos.y);
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
