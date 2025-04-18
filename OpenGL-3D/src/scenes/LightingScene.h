#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"
#include "../texture.h"

class LightingScene : public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, * m_LightShader;
	Camera* m_Camera;

	Cube m_Cube;
	Cube m_Plane;
	glm::vec3 m_CubePositions[6];

	// Cube Material
	Texture* m_ContainerTexture, *m_SpecularMap;
	glm::vec3 m_SpecularColor;
	int m_Shininess;
	
	// Lights
	glm::vec3 m_DirectionalLightDir;
	glm::vec3 m_DirectionalLightColor;

	glm::vec3 m_PointLight1Pos;
	glm::vec3 m_PointLight2Pos;
	glm::vec3 m_PointLightColor, m_PointLightColor2;

	// Spotlight
	glm::vec3 m_SpotLightColor;
	float m_CutOffAngleInside, m_CutOffAngleOutside;

public:
	LightingScene(GLFWwindow* window);
	~LightingScene();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();
};

