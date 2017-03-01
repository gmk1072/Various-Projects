#include "GameEntity.h"

GameEntity::GameEntity()
{
}

GameEntity::GameEntity(Mesh * _mesh)
{
	mesh = _mesh;
	translation = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	worldMatrix = XMFLOAT4X4();
}

GameEntity::GameEntity(Mesh * _mesh, Material * _mat)
{
	mesh = _mesh;
	translation = { 0,0,0 };
	rotation = { 0,0,0 };
	scale = { 1,1,1 };
	worldMatrix = XMFLOAT4X4();
	mat = _mat;
}


GameEntity::~GameEntity()
{
}

XMFLOAT4X4 GameEntity::getWorldMatrix()
{
	return worldMatrix;
}

XMFLOAT3 GameEntity::getTranslation()
{
	return translation;
}

XMFLOAT3 GameEntity::getRotation()
{
	return rotation;
}

XMFLOAT3 GameEntity::getScale()
{
	return scale;
}

Mesh * GameEntity::getMesh()
{
	return mesh;
}

void GameEntity::PrepareMaterial(XMFLOAT4X4 _view, XMFLOAT4X4 _projection)
{
	// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		mat->GetVertexShader()->SetMatrix4x4("world", getWorldMatrix());
		//vertexShader->SetMatrix4x4("view", viewMatrix);
		mat->GetVertexShader()->SetMatrix4x4("view", _view);
		//vertexShader->SetMatrix4x4("projection", projectionMatrix);
		mat->GetVertexShader()->SetMatrix4x4("projection", _projection);

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		mat->GetVertexShader()->CopyAllBufferData();
		mat->GetPixelShader()->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		mat->GetVertexShader()->SetShader();
		mat->GetPixelShader()->SetShader();

}

void GameEntity::setTranslation(float x, float y, float z)
{
	translation = { x,y,z };
}

void GameEntity::setRotation(float x, float y, float z)
{
	rotation = { x,y,z };
}

void GameEntity::setScale(float x, float y, float z)
{
	scale = { x,y,z };
}

void GameEntity::Move(float x, float y, float z)
{
	translation.x += x;
	translation.y += y;
	translation.z += z;
}

void GameEntity::MoveForward(float x, float y, float z)
{
	translation.x += x*sinf(rotation.x)*cosf(rotation.y);
	translation.y += y*sinf(rotation.x)*sinf(rotation.y);
	translation.z += z*cosf(rotation.x);
}

void GameEntity::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
}

void GameEntity::ScaleUniform(float x)
{
	scale.x *= x;
	scale.y *= x;
	scale.z *= x;
}

void GameEntity::ScaleNonUniform(float x, float y, float z)
{
	scale.x *= x;
	scale.y *= y;
	scale.z *= z;
}

void GameEntity::UpdateWorldMatrix()
{
	//worldMatrix = ;
	//XMMATRIX worldMat =;
	//XMStoreFloat4x4(&worldMatrix, XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(translation.x, translation.y, translation.z));
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationX(rotation.x) * XMMatrixRotationY(rotation.y) * XMMatrixRotationZ(rotation.z)*XMMatrixTranslation(translation.x, translation.y, translation.z)));
}
