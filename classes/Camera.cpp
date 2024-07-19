#include "Camera.h"

void Camera::updateCameraVectors()
{
	// Recalculate the front vector
	glm::vec3 f; 
	f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	f.y = sin(glm::radians(pitch));
	f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(f);
	// Recalculate the right vector
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
	: front(glm::vec3(0.f, 0.f, -1.f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM), position(position), worldUp(up), yaw(yaw), pitch(pitch)
{
	updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: position(glm::vec3(posX, posY, posZ)), worldUp(glm::vec3(upX, upY, upZ)), yaw(yaw), pitch(pitch)
{
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(position, position+front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD) position += front * velocity; 
	if (direction == BACKWARD) position -= front * velocity; 
	if (direction == LEFT) position -= right * velocity; 
	if (direction == RIGHT) position += right * velocity;
	if (direction == DOWN) position -= up * velocity;
	if (direction == UP) position += up * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	yoffset *= mouseSensitivity;
	xoffset *= mouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.f) pitch = 89.f;
		if (pitch < -89.f) pitch = -89.f;
	}

	updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.f) zoom = 1.f;
	if (zoom > 45.f) zoom = 45.f;
}

float Camera::getZoom() { return zoom; }



