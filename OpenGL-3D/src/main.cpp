#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "window.h"
#include "shader.h"
#include "camera.h"

#include "scenes/SceneManager.h"
#include "scenes/DefaultScene.h"
#include "scenes/LightTypes.h"
#include "scenes/BaseScene.h"


int main()
{
	Window* window = CreateBasicWindow();

	// Scenes
	SceneManager* defaultScene = new BaseScene(window->GetWindowInstance());

	SceneManager::CurrentScene = defaultScene;
	SceneManager::CurrentScene->Init();

	while (!window->GetWindowShouldClose())
	{
		window->PollEvents();

		SceneManager::CurrentScene->Update();
		SceneManager::CurrentScene->Render();

		window->Render();
	}

	delete window;

	return 0;
}
