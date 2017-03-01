#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "ShaderManager.h"
#include "InputManager.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

struct Vertex
{
	glm::vec3 loc;
	glm::vec2 uv;
	glm::vec3 normal;

	Vertex(glm::vec3 loc, glm::vec2 uv, glm::vec3 normal)
	{
		this->loc = loc;
		this->uv = uv;
		this->normal = normal;
	}
	Vertex() {}
};

struct VertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;
};

class Model
{
private:
	GLuint vertArr;
	unsigned int vertCount;
	GLuint vertBuf;
public:
	Model();
	~Model();
	bool buffer(std::string objFile);
	void render();
};

