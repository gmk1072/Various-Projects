#pragma once
#include "SimpleShader.h"

class Material
{
private:
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
public:
	Material();
	Material(SimpleVertexShader* _vertexShader, SimplePixelShader* _pixelShader);
	~Material();
	SimpleVertexShader* GetVertexShader();
	SimplePixelShader* GetPixelShader();
};

