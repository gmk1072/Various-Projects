#include "ShaderManager.h"



ShaderManager::ShaderManager()
{
	program = 0;
}


ShaderManager::~ShaderManager()
{
}

GLuint ShaderManager::getProgram() const
{
	return program;
}

bool ShaderManager::loadShaders(const char * vertexFile, const char * fragmentFile)
{
	GLuint g2 = loadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (g2 == 0) return 0;
	GLuint g = loadShader(vertexFile, GL_VERTEX_SHADER);
	if (g == 0) return 0;

	program = glCreateProgram();
	glAttachShader(program, g);
	glAttachShader(program, g2);
	glLinkProgram(program);
	GLint gint;
	glGetProgramiv(program,GL_LINK_STATUS , &gint);
	if (gint != 0) return true;
	else
	{
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* log = new char[logLength];
		glGetProgramInfoLog(program, logLength, 0, log);
		cout << log << endl;
		glDeleteProgram(program);
		delete[] log;
	}
	return false;
}

GLuint ShaderManager::loadShader(const char * file, GLenum shaderType)
{
	cout << file << endl;
	
	ifstream inFile(file, ios::binary);
	/*inFile.open(file, ios::binary);*/
	if (inFile.is_open())
	{
		inFile.seekg(0, ios::end);
		int length = (int)inFile.tellg();
		inFile.seekg(0, ios::beg);
		char* fileContents = new char[length + 1];
		inFile.read(fileContents, length);
		fileContents[length] = 0;
		inFile.close();

		GLuint g = glCreateShader(shaderType);
		glShaderSource(g, 1, &fileContents , 0);
		glCompileShader(g);
		GLint gint;
		glGetShaderiv(g, GL_COMPILE_STATUS, &gint);
		delete[] fileContents;
		if (gint == 0)
		{
			cout << "shader did not compile";
			GLint loglength;
			glGetShaderiv(g, GL_INFO_LOG_LENGTH, &loglength);
			GLchar* glchar = new char[loglength];

			glGetShaderInfoLog(g, loglength, 0, glchar);
			cout << glchar << endl;
			glDeleteShader(g);
			delete[] glchar;
			return 0;
		}
		else
		{
			return g;
		}
		
	}
	else
	{
		cout << "The file was not read." << endl;
		inFile.close();
		return 0;
	}
}
