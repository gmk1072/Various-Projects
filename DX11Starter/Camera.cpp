#include "Camera.h"



Camera::Camera()
{
	cameraPosition = XMFLOAT3(0,0,-5);
	cameraDirection = XMFLOAT3(0, 0, 1);
	xRot = 0;
	yRot = 0;
}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	XMVECTOR forward = { 0,0,1 };
	XMVECTOR right = { 1,0,0 };
	XMVECTOR up = { 0,1,0 };
	//cameraDirection = XMFLOAT3(0, 0, 1);
	XMStoreFloat3(&cameraDirection, XMVector3Rotate(forward, XMQuaternionRotationRollPitchYaw(xRot, yRot, 0)));
	XMVECTOR camdir = XMLoadFloat3(&cameraDirection);
	XMVECTOR LR = XMVector3Cross(camdir, { 0,1,0 });
	XMVECTOR campos = XMLoadFloat3(&cameraPosition);
	if (GetAsyncKeyState('1') & 0x8000) {
		speed-=deltaTime;
		if (speed < 1)
			speed = 1;
	}
	if (GetAsyncKeyState('2') & 0x8000) {
		speed += deltaTime;
		if (speed >15)
			speed = 15;
	}
	XMVECTOR change;
	if (GetAsyncKeyState('W') & 0x8000) {
		change = XMVector3Rotate(forward,camdir);
		campos += (deltaTime * speed * camdir);
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		change = XMVector3Rotate(forward, camdir);
		campos += (-deltaTime * speed * camdir);
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		
		campos += (deltaTime * speed * LR);
	}
	if (GetAsyncKeyState('D') & 0x8000) {
		campos += (-deltaTime * speed * LR);
	}
	if (GetAsyncKeyState('Z') & 0x8000) {
		change = { 0, deltaTime * speed , 0};
		campos += change;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		change = { 0, -deltaTime * speed , 0 };
		campos += change;
	}
	XMStoreFloat3(&cameraPosition, campos);
	//cameraPosition = XMLoadFloat3(&pos);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(XMMatrixLookToLH(campos, camdir, up)));
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return viewMatrix;
}

XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}
XMFLOAT3 Camera::getCameraPosition()
{
	return cameraPosition;
}
inline float clamp(float x, float a, float b)

{

	return x < a ? a : (x > b ? b : x);

}
void Camera::Rotate(float x, float y)
{
	
	xRot = x / 50;
	yRot = y / 50;
}

void Camera::updateProjectionMatrix(float width, float height)
{
	w = width;
	h = height;
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		1000.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}
