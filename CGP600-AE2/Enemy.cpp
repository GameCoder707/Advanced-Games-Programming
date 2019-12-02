#include "Enemy.h"

Enemy::Enemy(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale,  y_scale, z_scale);

	// Initialising the enemy's health
	m_health = 100.0f;

	// Initialising the enemy's movement speed
	m_move_speed = 2.0f;

}

Enemy::~Enemy()
{

}

// The enemy's health is set from the parameter
void Enemy::SetEnemyHealth(float amount)
{
	m_health = amount;
}

// Returns enemy's health
float Enemy::GetEnemyHealth()
{
	return m_health;
}

float Enemy::GetMoveSpeed()
{
	return m_move_speed;
}