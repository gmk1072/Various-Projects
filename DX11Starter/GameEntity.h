#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include "Mesh.h"
#include "Material.h"

using namespace DirectX;

class GameEntity
{
public:
	GameEntity();
	GameEntity(Mesh* _mesh);
	GameEntity(Mesh* _mesh, Material* _mat);
	~GameEntity();

	XMFLOAT4X4 getWorldMatrix();
	XMFLOAT3 getTranslation();
	XMFLOAT3 getRotation();
	XMFLOAT3 getScale();
	Mesh* getMesh();

	Material* mat;

	void PrepareMaterial(XMFLOAT4X4 _view, XMFLOAT4X4 _projection);
	void setTranslation(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	void Move(float x, float y, float z);
	void MoveForward(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void ScaleUniform(float x);
	void ScaleNonUniform(float x, float y, float z);

	void UpdateWorldMatrix();
private:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT3 translation;
	XMFLOAT3 rotation;
	XMFLOAT3 scale;
	Mesh* mesh;


};

