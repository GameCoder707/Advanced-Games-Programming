#include "Statik.h"

// Constructor
Statik::Statik(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

	// Initialising tree's health
	m_tree_health = 50.0f;

}

// Destructor
Statik::~Statik()
{

}

// Sets and Gets the tree's health
void Statik::SetTreeHealth(float value)
{
	m_tree_health = value;
}

float Statik::GetTreeHealth()
{
	return m_tree_health;
}