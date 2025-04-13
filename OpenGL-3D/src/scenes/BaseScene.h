#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"
#include "../texture.h"

class BaseScene : public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, * m_LightShader;
	Camera* m_Camera;

	Cube m_Cube;

	Texture* m_ContainerTexture;
public:
	BaseScene(GLFWwindow* window);
	~BaseScene();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();
};

