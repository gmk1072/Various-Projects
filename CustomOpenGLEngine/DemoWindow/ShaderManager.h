#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;
using std::cout;
using std::endl;
class ShaderManager
{
private:
	GLuint program;

public:
	ShaderManager();
	virtual ~ShaderManager();
	GLuint getProgram() const;
	bool loadShaders(const char* vertexFile, const char* fragmentFile);
	GLuint loadShader(const char* file, GLenum shaderType);
};

