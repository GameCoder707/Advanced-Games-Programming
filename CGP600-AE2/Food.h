#pragma once

#include "Asset.h"

class Food : public Asset
{
private:

	float m_health_value;

public:
	Food(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Food();

	float GetHealthValue();


};
