#pragma once

#include "Asset.h"

class Statik : public Asset
{
private:

	// This is the health of the tree
	float m_tree_health;

public:

	// Constructor and Destructor
	Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Statik();

	void SetTreeHealth(float value);
	float GetTreeHealth();


};
