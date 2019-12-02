#include "Player.h"

// Constructor
Player::Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the player asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);

	// Initialising the player's health
	// and initial health
	m_health = 100.0f;
	m_init_health = m_health;

	// Initialising booleans
	m_is_jumping = false;
	
	m_carrying_weapon = false;
	m_is_pushing = false;

	// Initialising player attributes
	m_move_speed = 4.0f;
	m_speed_multiplier = 1.0f;
	//m_jump_speed = 0.0010f;
	m_jump_speed = 7.0f;
	m_jump_height = 4.5f;

}

Player::~Player()
{

}

// DEFINITION OF THE SET AND GET FUNCTIONS

void Player::SetJumpState(bool state)
{
	m_is_jumping = state;
}

void Player::SetWeaponCarryingState(bool state)
{
	m_carrying_weapon = state;
}

void Player::SetPlayerHealth(float amount)
{
	m_health = amount;
}

void Player::ResetPlayerHealth()
{
	m_health = m_init_health;
}

void Player::SetPlayerMoveSpeed(float speed)
{
	m_move_speed = speed;
}

void Player::SetJumpHeight(float value)
{
	m_jump_height = value;
}

void Player::SetSpeedMultiplier(float amount)
{
	m_speed_multiplier = amount;
}

void Player::SetPushState(bool state)
{
	m_is_pushing = state;
}

bool Player::GetPushState()
{
	return m_is_pushing;
}

bool Player::GetJumpState()
{
	return m_is_jumping;
}

bool Player::GetWeaponCarryingState()
{
	return m_carrying_weapon;
}

float Player::GetPlayerHealth()
{
	return m_health;
}

float Player::GetMaxHealth()
{
	return m_init_health;
}

// We get the player's movement speed
// multiplied by the speed multiplier to
// get the accurate speed
float Player::GetPlayerMoveSpeed()
{
	return m_move_speed * m_speed_multiplier;
}

float Player::GetSpeedMultiplier()
{
	return m_speed_multiplier;
}

float Player::GetJumpSpeed()
{
	return m_jump_speed;
}

float Player::GetJumpHeight()
{
	return m_jump_height;
}