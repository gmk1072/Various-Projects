#include "Material.h"



Material::Material()
{
}

Material::Material(SimpleVertexShader * _vertexShader, SimplePixelShader * _pixelShader)
{
	vertexShader = _vertexShader;
	pixelShader = _pixelShader;
}


Material::~Material()
{
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}
