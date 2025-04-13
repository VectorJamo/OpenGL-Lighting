#include "LightingScene.h"
#include <cmath>

#include "../utils/stb_image.h"

#include <iostream>

LightingScene::LightingScene(GLFWwindow* window)
	:SceneManager(window), m_VAO(0), m_VBO(0), m_Shader(nullptr), m_LightShader(nullptr), m_Camera(nullptr)
{
}

LightingScene::~LightingScene()
{
	delete m_Shader;
	delete m_LightShader;
	delete m_Camera;
}

void LightingScene::Init()
{
	m_CubePositions[0] = glm::vec3(0.0f, 0.0f, -5.0f);
	m_CubePositions[1] = glm::vec3(2.0f, 0.0f, -5.0f);
	m_CubePositions[2] = glm::vec3(-2.0f, 1.0f, -3.0f);
	m_CubePositions[3] = glm::vec3(-2.0f, -2.0f, -3.0f);
	m_CubePositions[4] = glm::vec3(-2.0f, -2.0f, -5.0f);
	m_CubePositions[5] = glm::vec3(2.0f, 2.0f, -5.0f);

	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Cube.vertices), m_Cube.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (const void*)(sizeof(float) * 6));

	glBindVertexArray(0);

	// Shaders
	m_Shader = new Shader("src/shaders/light-scene-shaders/vs.glsl", "src/shaders/light-scene-shaders/fs.glsl");
	m_LightShader = new Shader("src/shaders/light-scene-shaders/vs-light-source.glsl", "src/shaders/light-scene-shaders/fs-light-source.glsl");

	// Camera
	m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// Cube Material
	m_SpecularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_Shininess = 128;

	// Lights
	m_DirectionalLightDir = glm::vec3(1.0f, -1.0f, 0.0f);
	m_DirectionalLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	m_PointLight1Pos = glm::vec3(-5.0f, 0.0f, -3.0f);
	m_PointLight2Pos = glm::vec3(0.0f, -5.0f, -5.0f);
	m_PointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// Textures
	m_ContainerTexture = new Texture("res/container.png");
	m_ContainerTexture->Bind();

	glEnable(GL_DEPTH_TEST);
}

void LightingScene::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ProcessInput();

	double mx, my;
	glfwGetCursorPos(m_Window, &mx, &my);
	m_Camera->Update((float)mx, (float)my);
}

void LightingScene::Render()
{
	glm::mat4 view = m_Camera->GetViewMatrix();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Draw lights
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(m_PointLight1Pos);

	glBindVertexArray(m_VAO);
	m_LightShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

	model = glm::translate(m_PointLight2Pos);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

	// Draw objects
	m_Shader->Use();
	for (int i = 0; i < 6; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(m_CubePositions[i]);

		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
		
		glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.diffuse"), 0);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.specular"), 1, &m_SpecularColor[0]);
		glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.shininess"), m_Shininess);

		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "eyePos"), 1, &m_Camera->GetCameraPosition()[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "directionalLightDir"), 1, &m_DirectionalLightDir[0]);

		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "directionalLightColor"), 1, &m_DirectionalLightColor[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	}
}

void LightingScene::ProcessInput()
{
	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(m_Window, true);
	}
	if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Camera->MoveForward();
	}
	if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Camera->MoveLeft();
	}
	if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Camera->MoveBackward();
	}
	if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Camera->MoveRight();
	}
}