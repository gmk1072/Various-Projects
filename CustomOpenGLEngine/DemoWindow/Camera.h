#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <FreeImage.h>
#include <vector>
#include "InputManager.h"
class Camera
{
	//for updating
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

public:
	Camera();
	~Camera();
	//for look at matrix
	glm::vec3 eye;
	glm::vec3 center;
	glm::vec3 up;
	//rotation matrix
	glm::mat3 rotMat;
	//for aspect ratio
	float zoom;
	int width;
	int height;
	//for perspective matrix
	float fovy;
	float aspectRatio;
	float zNear;
	float zFar;
	//updating camera
	Transform transform;
	RigidBody rigidBody;
	//matrices
	glm::mat4 lookAtMat;
	glm::mat4 perspectiveMat;
	//will be updated and uploaded
	glm::mat4 camMat;
	glm::vec3 camRot;
	void update(float dt, GLFWwindow *window);
};

