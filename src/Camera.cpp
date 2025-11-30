#include "../include/Logger.hpp"
#include "../include/Camera.hpp"
#include "../include/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up) :
 Position(position),
 Front(glm::vec3(0.0f, 0.0f, -1.0f)),
 Up(glm::vec3(0.0f, 1.0f, 0.0f)),
 Right(glm::vec3(1.0f, 0.0f, 0.0f)),
 WorldUp(up),
 Orientation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
 Yaw(0.0f),
 Pitch(0.0f),
 MovementSpeed(2.5f),
 MouseSensitivity(0.1f) {
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::update(const InputManager& input, float deltaTime) {
  float velocity = MovementSpeed * deltaTime;

  // Keyboard Movement
  if (input.isKeyPressed(GLFW_KEY_W))
    Position += Front * velocity;
  if (input.isKeyPressed(GLFW_KEY_S))
    Position -= Front * velocity;
  if (input.isKeyPressed(GLFW_KEY_A))
    Position -= Right * velocity;
  if (input.isKeyPressed(GLFW_KEY_D))
    Position += Right * velocity;
  if (input.isKeyPressed(GLFW_KEY_SPACE))
    Position += WorldUp * velocity;
  if (input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
    Position -= WorldUp * velocity;

  // Get Mouse Position
  auto [mouseX, mouseY] = input.getMousePosition();

  // Mouse Rotation
  auto [deltaX, deltaY] = input.getMouseDelta();
  LOG_INFO_F("Mouse moving: {}, {}", deltaX, deltaY);

  // Mouse to radians
  if (deltaX != 0.0 || deltaY != 0.0) {
    // Accumulate Angles
    Yaw += static_cast<float>(-deltaX) * MouseSensitivity;
    Pitch += static_cast<float>(deltaY) * MouseSensitivity;

    // Clamp putch to prevent flipping upside down
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;

    // Quaternion for each rotations Yaw & Pitch
    glm::quat yawRotation = glm::angleAxis(glm::radians(Yaw), WorldUp);
    glm::quat pitchRotation = glm::angleAxis(glm::radians(Pitch), Right);

    // Apply the rotations
    Orientation = yawRotation * pitchRotation;

    // normalize for drifting
    Orientation = glm::normalize(Orientation);

    // Recalculate the camera vectors based on new angles
    updateCameraVectors();
  }
}

void Camera::updateCameraVectors() {
  Front = glm::normalize(Orientation * glm::vec3(0.0f, 0.0f, -1.0f));
  Right = glm::normalize(Orientation * glm::vec3(1.0f, 0.0f, 0.0f));
  Up = glm::normalize(Orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}
