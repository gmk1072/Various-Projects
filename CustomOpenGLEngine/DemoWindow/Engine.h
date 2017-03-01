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
#include <ctime>
#include "Camera.h"
#include "Model.h"
enum CollisionType { COLLIDERLESS, AABB, SPHERE };
enum ObjectType { BOMB, BOMBER, BUCKETS, GROUND, OBJECT};
//struct Vertex
//{
//	glm::vec3 loc;
//	glm::vec2 uv;
//	glm::vec3 normal;
//};

struct Transform
{
	glm::vec3 loc, rot, size;
	glm::mat4 matrix;
};

struct RigidBody
{
	glm::vec3 velocity;
	glm::vec3 force;
	float mass;
};

struct Object
{
	char* filename;
	Transform transform;
	RigidBody rigidBody;
	GLuint texid;
	CollisionType collisionType;
	ObjectType objectType;
};

class Engine
{
private :
	GLFWwindow* GLFWwindowPtr;
	GLuint vertBuf;
	ShaderManager shade;
	unsigned int uvVertCount;
	int index = 0;
	vector<Object> objects;
	float t;
	float pt;
	float dt;
	int objectAmount;
	int player;
	int bomber;
	int ground;
	Camera camera;
	Model model;
public:
	Engine();
	~Engine();
	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	bool useTextures(Object& o);
	bool collides(const Object &o1, const Object &o2);
};

