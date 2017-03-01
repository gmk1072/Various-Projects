#include "Camera.h"

Camera::Camera()
{
	transform.loc = { 0,0,1.5 };
	transform.rot = { 0,0,0 };
	rotMat = (glm::mat3)glm::yawPitchRoll(transform.rot.y, transform.rot.x, transform.rot.z);

	eye = transform.loc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);

	lookAtMat = glm::lookAt(eye, center, up);

	zoom = 1.f;
	width = 800;
	height = 600;

	fovy = 3.14150f * .4f / zoom;
	aspectRatio = (float)width / (float)height;
	zNear = .01f;
	zFar = 1000.f;

	perspectiveMat = glm::perspective(fovy, aspectRatio, zNear, zFar);

	camRot = glm::vec3();

}


Camera::~Camera()
{
}

void Camera::update(float dt, GLFWwindow *window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	camRot.y -= .005 * (x - 800 * .5f); //Yaw
	camRot.x -= .005 * (y - 600 * .5f); //Pitch
	camRot.x = glm::clamp(camRot.x, -.5f * glm::pi<float>(), .5f * glm::pi<float>());

	glfwSetCursorPos(window, 800 * .5f, 600 * .5f);

	rigidBody.force = glm::vec3();

	// Move with arrows
	glm::vec3 dv;
	glm::mat3 R = (glm::mat3)glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);
	if (InputManager::getKey(GLFW_KEY_LEFT))
		dv += R * glm::vec3(-1, 0, 0);
	if (InputManager::getKey(GLFW_KEY_RIGHT))
		dv += R * glm::vec3(1, 0, 0);
	if (InputManager::getKey(GLFW_KEY_UP))
		dv += R * glm::vec3(0, 0, -1);
	if (InputManager::getKey(GLFW_KEY_DOWN))
		dv += R * glm::vec3(0, 0, 1);
	float speed = .1f;
	if (InputManager::getKey(GLFW_KEY_LEFT_SHIFT) || InputManager::getKey(GLFW_KEY_RIGHT_SHIFT))
	{
		speed = .3f;
	}
	else
	{
		speed = .1f;
	}
	if (dv != glm::vec3())
		dv = glm::normalize(dv) * speed;



	rigidBody.velocity *= .8;

	transform.matrix = glm::translate(transform.loc)*glm::yawPitchRoll(transform.rot.y + camRot.y, transform.rot.x + camRot.x, transform.rot.z + camRot.z)*glm::scale(transform.size);

	glm::vec3 dl;
	//dv = rigidBody.force / rigidBody.mass * dt;
	rigidBody.velocity += dv;
	dl = rigidBody.velocity * dt;
	transform.loc += dl;

	rotMat = (glm::mat3) glm::yawPitchRoll(camRot.y, camRot.x, camRot.z);

	eye = transform.loc;
	center = eye + rotMat * glm::vec3(0, 0, -1);
	up = rotMat * glm::vec3(0, 1, 0);

	glm::mat4 viewMat = glm::lookAt(eye, center, up);

	camMat = perspectiveMat * viewMat;


	glUniformMatrix4fv(3, 1, GL_FALSE, &camMat[0][0]);
}
