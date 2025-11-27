#include "../include/Camera.hpp"
#include "../include/InputManager.hpp"
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
 Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(2.5f), MouseSensitivity(0.1f) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
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

  // Mouse Rotation
  auto [mouseX, mouseY] = input.getMousePosition();
}

void Camera::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Yaw));
  front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);

  // Also re-calculate the Right and Up vector
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
