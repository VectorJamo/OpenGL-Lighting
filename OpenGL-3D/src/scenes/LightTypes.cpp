#include "LightTypes.h"
#include <cmath>

#include "../utils/stb_image.h"

#include <iostream>

LightTypes::LightTypes(GLFWwindow* window)
	:SceneManager(window), m_VAO(0), m_VBO(0), m_Shader(nullptr), m_LightShader(nullptr), m_Camera(nullptr)
{
}

LightTypes::~LightTypes()
{
	delete m_Shader;
	delete m_LightShader;
	delete m_Camera;
}

void LightTypes::Init()
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

	m_CubePositions[0] = glm::vec3(0.0f, 0.0f, -5.0f);
	m_CubePositions[1] = glm::vec3(2.0f, 2.0f, -5.0f);
	m_CubePositions[2] = glm::vec3(4.f, 2.0f, -10.0f);
	m_CubePositions[3] = glm::vec3(-2.0f, 2.0f, -8.0f);
	m_CubePositions[4] = glm::vec3(4.0f, -2.0f, -5.0f);

	// Shaders
	m_Shader = new Shader("src/shaders/light-types-vs.glsl", "src/shaders/light-types-fs.glsl");
	m_LightShader = new Shader("src/shaders/light-vs.glsl", "src/shaders/light-fs.glsl");

	glEnable(GL_DEPTH_TEST);

	// Camera
	m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// Lights
	m_LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ObjectColor = glm::vec3(0.0f, 1.0f, 0.0f);
	m_SpecularHighlightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	// Textures
	glGenTextures(1, &m_DiffuseMap);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_DiffuseMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load in the textures
	int width, height, colorChannels;
	unsigned char* data = stbi_load("res/container.png", &width, &height, &colorChannels, 0); // Diffuse Map
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	data = stbi_load("res/container_specular.png", &width, &height, &colorChannels, 0); // Specular Map

	glGenTextures(1, &m_SpecularMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_SpecularMap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

void LightTypes::Update()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ProcessInput();

	double mx, my;
	glfwGetCursorPos(m_Window, &mx, &my);
	m_Camera->Update((float)mx, (float)my);

	// Directional Light 
	m_DirectionalLight = glm::vec3(0.0f, -1.0f, 0.0f);

	// Point Light
	m_LightPosition = glm::vec3(5.0f, 3.0f, -6.0f); // Offset by -5 to revolve around the object
}

void LightTypes::Render()
{
	glm::mat4 view = m_Camera->GetViewMatrix();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	// Draw object
	for (int i = 0; i < 5; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_CubePositions[i]);

		glBindVertexArray(m_VAO);
		m_Shader->Use();
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "lightPosition"), 1, &m_LightPosition[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "directionalLight"), 1, &m_DirectionalLight[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "spotLightDirection"), 1, &m_Camera->GetCameraDirection()[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "lightColor"), 1, &m_LightColor[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "objectColor"), 1, &m_ObjectColor[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "cameraPosition"), 1, &m_Camera->GetCameraPosition()[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.ambient"), 1, &m_ObjectColor[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.diffuse"), 1, &m_ObjectColor[0]);
		glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.specular"), 1, &m_SpecularHighlightColor[0]);
		glUniform1f(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.shininess"), 128.0f);
		glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "material.specular"), 1);
		glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	}

	// Draw light source
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_LightPosition);

	glBindVertexArray(m_VAO);
	m_LightShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform3fv(glGetUniformLocation(m_LightShader->GetShaderProgram(), "lightColor"), 1, &m_LightColor[0]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
}

void LightTypes::ProcessInput()
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

void LightTypes::InitObject()
{
}

void LightTypes::InitLight()
{
}
