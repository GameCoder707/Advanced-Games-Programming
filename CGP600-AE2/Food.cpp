#include "Food.h"

Food::Food(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

	m_health_value = 10.0f;
}

Food::~Food()
{

}

float Food::GetHealthValue()
{
	return m_health_value;
}