#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/vector_angle.hpp>

class Camera
{
private:
	glm::vec3 m_CamPosition, m_CamDirection;
	glm::mat4 m_ViewMatrix;

	float m_lastMouseX, m_lastMouseY;

	const glm::vec3 m_WorldUp;
	bool m_FirstFrame;

	const float m_CameraSpeed = 0.2f;
	const float m_CameraMovementSpeed = 0.01f;

public:
	Camera(const glm::vec3& camPosition, const glm::vec3& camDirection);
	~Camera();

	void Update(int mouseX, int mouseY);
	// Yes, there are much better ways to do this but this is just a test camera class....
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();

	inline const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
	inline const glm::vec3& GetCameraPosition() { return m_CamPosition; }
	inline const glm::vec3& GetCameraDirection() { return m_CamDirection; }

};