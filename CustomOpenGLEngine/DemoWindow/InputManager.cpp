#include "InputManager.h"


std::map<int, bool> InputManager::keyDown;
InputManager::InputManager()
{
}


InputManager::~InputManager()
{
}

void InputManager::init(GLFWwindow * window)
{
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseClick);
}

void InputManager::mouseClick(GLFWwindow * window, int key, int action, int mods)
{
	keyDown[key] = action;
}

void InputManager::keyCallback(GLFWwindow * window, int key, int scanCode, int action, int mods)
{
	keyDown[key] = action;
}

void InputManager::releaseKey(int key)
{
	keyDown[key] = false;
}

int InputManager::getKey(int key)
{
	return keyDown[key];
}
