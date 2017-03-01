#pragma once
#include "GameEntity.h"

class Camera
{
private:
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;
	XMFLOAT3 cameraDirection;
	XMFLOAT3 cameraPosition;
	float xRot;
	float yRot;
	float w;
	float h;
public:
	Camera();
	~Camera();

	float speed = 10;
	void Update(float deltaTime);
	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4X4 GetProjectionMatrix();
	XMFLOAT3 getCameraPosition();
	void Rotate(float x, float y);
	void updateProjectionMatrix(float width, float height);
};

