#pragma once
#include "SceneManager.h"
#include "../shader.h"
#include "../camera.h"
#include "../objects/Cube.h"
#include "../texture.h"

#include "../models/Model.h"
#include "../models/Terrian.h"

class SimpleWorld : public SceneManager
{
private:
	// Scene state variables
	unsigned int m_VAO, m_VBO;
	Shader* m_Shader, * m_LightShader;
	Camera* m_Camera;

	Cube m_Cube;

	Texture* m_ContainerTexture, *m_FloorTexture;

	// Terrian	
	std::vector<Terrian*> m_Terrian;

public:
	SimpleWorld(GLFWwindow* window);
	~SimpleWorld();

	void Init() override;
	void Update() override;
	void Render() override;
	void ProcessInput();
};

