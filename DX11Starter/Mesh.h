#pragma once
#include <d3d11.h>
#include "Vertex.h"
using namespace DirectX;

class Mesh
{


public:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int numIndices;
	Mesh();
	Mesh(Vertex* _verts, int _vertCount, unsigned int* _indices, int _numIndices, ID3D11Device* _devObj);
	Mesh(char*, ID3D11Device*);
	~Mesh();

	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	int GetIndexCount();

	void createBuffers(Vertex* _verts, int _vertCount, unsigned int* _indices, int _numIndices, ID3D11Device* _devObj);
};

