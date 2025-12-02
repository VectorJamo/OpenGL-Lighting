#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"
#include "../texture.h"

class InteractiveScene : public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, * m_LightShader;
	Camera* m_Camera;

	// Objects
	Cube m_Cube;
	Cube m_Plane;

	// Colors
	glm::vec3 m_FloorColor;

	// Textures
	Texture* m_ContainerTexture;

public:
	InteractiveScene(GLFWwindow* window);
	~InteractiveScene();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();
};

