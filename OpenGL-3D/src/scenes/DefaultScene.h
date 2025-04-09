#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"

class DefaultScene: public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, *m_LightShader;
	Camera* m_Camera;

	Cube m_Cube;

	// Light
	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColor;
	glm::vec3 m_ObjectColor;
	glm::vec3 m_SpecularHighlightColor;

	// Textures
	unsigned int m_DiffuseMap;
	unsigned int m_SpecularMap;

public:
	DefaultScene(GLFWwindow* window);
	~DefaultScene();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();

	// Scene specific
	void InitObject();
	void InitLight();
};

