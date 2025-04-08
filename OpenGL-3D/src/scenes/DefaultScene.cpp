#include "DefaultScene.h"
#include <cmath>

#include <iostream>

DefaultScene::DefaultScene(GLFWwindow* window)
	:SceneManager(window), m_VAO(0), m_VBO(0), m_Shader(nullptr), m_LightShader(nullptr), m_Camera(nullptr)
{
}

DefaultScene::~DefaultScene()
{
	delete m_Shader;
	delete m_LightShader;
	delete m_Camera;
}

void DefaultScene::Init()
{
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Cube.vertices), m_Cube.vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (const void*)(sizeof(float) * 3));

	glBindVertexArray(0);

	// Shaders
	m_Shader = new Shader("src/shaders/vs.glsl", "src/shaders/fs.glsl");
	m_LightShader = new Shader("src/shaders/light-vs.glsl", "src/shaders/light-fs.glsl");

	glEnable(GL_DEPTH_TEST);

	// Camera
	m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// Lights
	m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ObjectColor = glm::vec3(0.0f, 1.0f, 0.0f);
	m_SpecularHighlightColor = glm::vec3(1.0f, 1.0f, 1.0f);
}

void DefaultScene::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ProcessInput();

	double mx, my;
	glfwGetCursorPos(m_Window, &mx, &my);
	m_Camera->Update((float)mx, (float)my);

	// Light's motion
	float radius = 5.0f;
	float speed = 2.0f;
	float lightX = radius * cos(speed * glfwGetTime());
	float lightZ = radius * sin(speed * glfwGetTime());

	m_LightPosition = glm::vec3(lightX, 0.0f, lightZ - 5.0f); // Offset by -5 to revolve around the object
}

void DefaultScene::Render()
{
	// Draw object
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 view = m_Camera->GetViewMatrix();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	glBindVertexArray(m_VAO);
	m_Shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "lightPosition"), 1, &m_LightPosition[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "lightColor"), 1, &m_LightColor[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "objectColor"), 1, &m_ObjectColor[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "cameraPosition"), 1, &m_Camera->GetCameraPosition()[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.ambient"), 1, &m_ObjectColor[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.diffuse"), 1, &m_ObjectColor[0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.specular"), 1, &m_SpecularHighlightColor[0]);
	glUniform1f(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.shininess"), 128.0f);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);

	// Draw light source
	model = glm::mat4(1.0f);
	model = glm::translate(model, m_LightPosition);
	
	m_LightShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "lightColor"), 1, &m_LightColor[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
}

void DefaultScene::ProcessInput()
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

void DefaultScene::InitObject()
{
}

void DefaultScene::InitLight()
{
}
