#include "camera.h"

#include <iostream>

Camera::Camera(const glm::vec3& camPosition, const glm::vec3& camDirection)
	:m_CamPosition(camPosition), m_CamDirection(camDirection), m_lastMouseX(0.0f), m_lastMouseY(0.0f), m_WorldUp(0.0f, 1.0f, 0.0f),
	m_FirstFrame(true)
{
	m_ViewMatrix = glm::mat4(1.0f);
	m_ViewMatrix = glm::lookAt(camPosition, camPosition + camDirection, m_WorldUp);
}

Camera::~Camera()
{
}

void Camera::Update(int mouseX, int mouseY)
{
	int mouseDX, mouseDY;
	if (m_FirstFrame)
	{
		m_lastMouseX = mouseX;
		m_lastMouseY = mouseY;
		m_FirstFrame = false;

		return;
	}
	else {
		mouseDX = mouseX - m_lastMouseX;
		mouseDY = mouseY - m_lastMouseY;
	}

	m_lastMouseX = mouseX;
	m_lastMouseY = mouseY;

	glm::mat4 rotation = glm::mat4(1.0f);
	rotation = glm::rotate(rotation, -glm::radians(m_CameraSpeed * (float)mouseDX), m_WorldUp); // For horizontal camera rotation

	m_CamDirection = glm::mat3(rotation) * m_CamDirection;

	glm::vec3 cameraRight = glm::normalize(glm::cross(m_CamDirection, m_WorldUp)); // For vertical camera rotation
	rotation = glm::rotate(rotation, -glm::radians(m_CameraSpeed * (float)mouseDY), cameraRight);
	
	glm::vec3 camDirection = glm::mat3(rotation) * m_CamDirection;
	
	float verticalAngle = abs(glm::degrees(glm::angle(m_WorldUp, camDirection)));
	if (verticalAngle > 10.0f && verticalAngle < 170.0f)
	{
		m_CamDirection = camDirection;
	}

	m_ViewMatrix = glm::lookAt(m_CamPosition, m_CamPosition + m_CamDirection, m_WorldUp);
}

void Camera::MoveForward()
{
	m_CamPosition += m_CameraMovementSpeed * m_CamDirection;
}

void Camera::MoveBackward()
{
	m_CamPosition -= m_CameraMovementSpeed * m_CamDirection;
}

void Camera::MoveLeft()
{
	glm::vec3 m_CameraRight = glm::normalize(glm::cross(m_CamDirection, m_WorldUp));
	m_CamPosition -= m_CameraMovementSpeed * m_CameraRight;
}

void Camera::MoveRight()
{
	glm::vec3 m_CameraRight = glm::normalize(glm::cross(m_CamDirection, m_WorldUp));
	m_CamPosition += m_CameraMovementSpeed * m_CameraRight;
}
