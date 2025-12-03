#include "SimpleWorld.h"
#include <cmath>

#include "../utils/stb_image.h"

#include <iostream>

#include <cstdlib>
#include <ctime>

SimpleWorld::SimpleWorld(GLFWwindow* window)
	:SceneManager(window), m_VAO(0), m_VBO(0), m_Shader(nullptr), m_LightShader(nullptr), m_Camera(nullptr)
{

	srand(time(nullptr));
}

SimpleWorld::~SimpleWorld()
{
	for (auto& terrian : m_Terrian)
		delete terrian;

	delete m_TreeModel;

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
	m_Shader = new Shader("src/shaders/world/vs.glsl", "src/shaders/world/fs.glsl");

	// Camera
	m_Camera = new Camera(glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));

	// Lights

	// Textures
	m_FloorTexture = new Texture("res/dirt.jpg", GL_RGB);
	m_TreeTexture = new Texture("res/tree_texture.png", GL_RGBA);

	// Terrian
	m_Identity = glm::mat4(1.0f);
	
	int numTerriansX = 6;
	int numTerriansZ = 6;

	for (int i = 0; i <= numTerriansZ; i++)
	{
		int gridZ = 3 - i;
		for (int j = 0; j <= numTerriansX; j++)
		{
			int gridX = 3 - j;
			m_Terrian.emplace_back(new Terrian(gridX, gridZ));
		}
	}

	// Models
	m_TreeModel = new Model("res/models/SimpleTree.obj");
	
	m_NumTrees = 100;
	m_TreePositions.reserve(m_NumTrees);

	int totalTerrianX = numTerriansX * Terrian::GetTerrianSize();
	int totalTerrianZ = numTerriansX * Terrian::GetTerrianSize();

	for (int i = 0; i < m_NumTrees; i++)
	{
		float xpos = (totalTerrianX/2) - rand() % totalTerrianX;
		float zpos = (totalTerrianZ/2) - rand() % totalTerrianZ;

		m_TreePositions.emplace_back(glm::vec3(xpos, 0.0f, zpos));
	}

	m_SkyColor = glm::vec4(0.2f, 0.8f, 0.8f, 1.0f);

	glClearColor(m_SkyColor.x, m_SkyColor.g, m_SkyColor.b, m_SkyColor.a);
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
	glm::mat4 view = m_Camera->GetViewMatrix();

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(60.0f), 800.0f / 600.0f, 0.1f, 1000.0f);

	m_FloorTexture->Bind(0);

	m_Shader->Use();

	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &m_Identity[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "projection"), 1, GL_FALSE, &projection[0][0]);
	glUniform1i(glGetUniformLocation(m_Shader->GetShaderProgram(), "textureUnit"), 0);
	glUniform3fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "cameraPosition"), 1, &m_Camera->GetCameraPosition()[0]);
	glUniform4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "skyColor"), 1, &m_SkyColor.x);

	for (auto& terrian : m_Terrian) 
	{
		glBindVertexArray(terrian->GetTerrianVAO());
	
		glDrawElements(GL_TRIANGLES, terrian->GetTerrianIndices().size(), GL_UNSIGNED_SHORT, 0);
	}
	m_TreeTexture->Bind(0);
	glBindVertexArray(m_TreeModel->GetModelVAO());

	for (int i = 0; i < m_NumTrees; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(m_TreePositions[i]);
		model = glm::scale(model, glm::vec3(5.0f, 8.0f, 5.0f));

		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "model"), 1, GL_FALSE, &model[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(m_Shader->GetShaderProgram(), "view"), 1, GL_FALSE, &view[0][0]);
		glDrawElements(GL_TRIANGLES, m_TreeModel->GetModelIndices().size(), GL_UNSIGNED_SHORT, 0);
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