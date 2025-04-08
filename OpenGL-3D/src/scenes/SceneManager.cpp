#include "SceneManager.h"

SceneManager* SceneManager::CurrentScene;
SceneManager::SceneManager(GLFWwindow* window)
	:m_Window(window)
{
}
