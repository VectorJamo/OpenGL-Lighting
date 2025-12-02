#include "SimpleWorld.h"
#include <cmath>

#include "../utils/stb_image.h"

#include <iostream>

SimpleWorld::SimpleWorld(GLFWwindow* window)
	:SceneManager(window), m_VAO(0), m_VBO(0), m_Shader(nullptr), m_LightShader(nullptr), m_Camera(nullptr)
{
}

SimpleWorld::~SimpleWorld()
{
	for (auto& terrian : m_Terrian)
		delete terrian;

	delete m_Shader;
	delete m_LightShader;
	delete m_Camera;

}

void SimpleWorld::Init()
{
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
	m_Shader = new Shader("src/shaders/base/vs.glsl", "src/shaders/base/fs.glsl");

	// Camera
	m_Camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// Lights

	// Textures
	m_FloorTexture = new Texture("res/dirt.jpg", GL_RGB);
	m_FloorTexture->Bind(0);

	// Terrian
	m_Terrian.emplace_back(new Terrian(0.0f, 0.0f));
	m_Terrian.emplace_back(new Terrian(-1.0f, 0.0f));
	m_Terrian.emplace_back(new Terrian(-1.0f, 1.0f));
	m_Terrian.emplace_back(new Terrian(0.0f, 1.0f));

	glClearColor(0.2f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);
}

void SimpleWorld::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ProcessInput();

	double mx, my;
	glfwGetCursorPos(m_Window, &mx, &my);
	m_Camera->Update((float)mx, (float)my);
}

void SimpleWorld::Render()
{
	// Draw object
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 view = m_Camera->GetViewMatrix();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	m_Shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "textureUnit"), 0);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "cameraPosition"), 1, &m_Camera->GetCameraPosition()[0]);
	
	for (auto& terrian : m_Terrian) 
	{
		glBindVertexArray(terrian->GetTerrianVAO());
	
		glDrawElements(GL_TRIANGLES, terrian->GetTerrianIndices().size(), GL_UNSIGNED_SHORT, 0);
	}
}

void SimpleWorld::ProcessInput()
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