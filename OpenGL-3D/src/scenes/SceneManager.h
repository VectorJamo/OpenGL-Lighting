#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class SceneManager
{
protected:
	GLFWwindow* m_Window;

public:
	static SceneManager* CurrentScene;

	SceneManager(GLFWwindow* window);

	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

