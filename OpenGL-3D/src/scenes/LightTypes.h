#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"

class LightTypes : public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, *m_LightShader, *m_PlaneShader;
	Camera* m_Camera;

	Cube m_Cube;
	glm::vec3 m_CubePositions[5];

	// TODO: Plane
	glm::vec3 m_PlaneTopLeftLocation;
	glm::vec3 m_PlaneWidth;
	glm::vec3 m_PlaneHeight;

	// Light
	glm::vec3 m_LightPosition;
	glm::vec3 m_LightColor;
	glm::vec3 m_ObjectColor;
	glm::vec3 m_SpecularHighlightColor;
	glm::vec3 m_DirectionalLight;

	// Textures
	unsigned int m_DiffuseMap;
	unsigned int m_SpecularMap;

public:
	LightTypes(GLFWwindow* window);
	~LightTypes();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();

	// Scene specific
	void InitObject();
	void InitLight();
};

