#pragma once
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>

class InputManager
{
public:
	InputManager();
	virtual ~InputManager();

	static std::map<int, bool> keyDown;
	static void init(GLFWwindow* window);
	static void mouseClick(GLFWwindow* window, int key, int action, int mods);
	static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods);
	static void releaseKey(int key);
	static int getKey(int key);
};

