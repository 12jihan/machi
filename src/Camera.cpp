#include "../include/Logger.hpp"
#include "../include/Camera.hpp"
#include "../include/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up) :
 Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f) {
  Position = position;
  WorldUp = up;
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
  // glm::mat4 rotation = glm::mat4_cast(Orientation);
  // glm::mat4 translation = glm::translate(glm::mat4(1.0f), -Position);

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
    float yawAngle = glm::radians(static_cast<float>(-deltaX) * MouseSensitivity);
    float pitchAngle = glm::radians(static_cast<float>(-deltaY) * MouseSensitivity);

    // Quaternion for each rotations
    // Yaw
    glm::quat yawRotation = glm::angleAxis(yawAngle, WorldUp);

    // Pitch
    glm::quat pitchRotation = glm::angleAxis(pitchAngle, Right);

    // Apply the rotations
    Orientation = yawRotation * Orientation * pitchRotation;

    // normalize for drifting
    Orientation = glm::normalize(Orientation);

    // Recalculate the camera vectors based on new angles
    updateCameraVectors();
  }
}

void Camera::updateCameraVectors() {
  Front = glm::normalize(Orientation * glm::vec3(0.0f, 0.0f, -8.0f));
  Right = glm::normalize(Orientation * glm::vec3(1.0f, 0.0f, 0.0f));
  Up = glm::normalize(Orientation * glm::vec3(0.0f, 1.0f, 0.0f));
}
