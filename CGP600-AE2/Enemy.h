#pragma once

// Including the asset class to load
// and draw the model
#include "Asset.h"

class Enemy : public Asset
{
private:

	// This stores the enemy's health
	float m_health;

	// This is the speed of the enemy
	float m_move_speed;

public:
	// Class Constructor and Destructor
	Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Enemy();

	// Set and Get functions for 
	// enemy's health and speed
	void SetEnemyHealth(float amount);
	float GetEnemyHealth();

	float GetMoveSpeed();


};
