#pragma once

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	DOWN,
	UP,
	NONE
};

// DEFAUT VALUES
const float YAW		= -90.f;
const float PITCH = 0.f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.f;

class Camera
{
private:
	// Attrib
	glm::vec3 position,
			  front, 
			  up,
			  right,
			  worldUp;
	// euler angles
	float yaw,
		pitch; 
	// Camera Options
	float movementSpeed,
		mouseSensitivity,
		zoom;

	void updateCameraVectors(); 
public:
	Camera(glm::vec3 position = glm::vec3(0.f, 0.f, 0.f), glm::vec3 up = glm::vec3(0.f, 1.f, 0.f), float yaw = YAW, float pitch = PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
	glm::mat4 getViewMatrix();
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yoffset);
	float getZoom();
};

