#pragma once
#include "Camera.h"
struct DirectionalLight {
	XMFLOAT4 AmbientColor;
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Direction;
};

struct PointLight{
	XMFLOAT4 DiffuseColor;
	XMFLOAT3 Position;
};