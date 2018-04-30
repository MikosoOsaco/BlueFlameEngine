#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

namespace ENGINE {

	// Enum for camera movement
	enum Camera_Movement 
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVTY = 0.1f;
	const float ZOOM = 75.0f;

	class Camera
	{
	public:
		// Camera Attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;
		// Eular Angles
		float Yaw;
		float Pitch;
		// Camera options
		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		float lastX = 1280.0f / 2.0f;
		float lastY = 720.0f / 2.0f;
		bool firstMouse = true;

		float height = 720.0f;
		float width = 1280.0f;

		// Constructor with vectors
		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		// Constructor with scalar values
		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

		// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
		glm::mat4 GetViewMatrix();

		// Processes input based on a direction
		void ProcessKeyboard(Camera_Movement direction, float deltaTime);
		// Processes input from mouse
		void ProcessMouseMovement(float eventX, float eventY, bool constrainPitch = true);
		// Processes input from mouse wheel
		void ProcessMouseScroll(float yoffset);

		// Set the X and Y rotations
		void SetRotationY(float yaw);
		void SetRotationX(float pitch);

	private:
		// Calculates the front vector from the Camera's (updated) Eular Angles
		void updateCameraVectors();		
	};
}
#endif