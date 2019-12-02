#pragma once

// Including the asset header
// file because the player is an
// asset
#include "Asset.h"

class Player : public Asset
{
private:

	// This is the movement speed of the player
	float m_move_speed;

	// This is to determine how fast or slow
	// the player can go from the base speed
	float m_speed_multiplier;

	// This determines the health 
	// of the player
	float m_health;

	// This is the inital health of
	// the player
	float m_init_health;

	// This is to determine if the
	// player is jumping
	bool m_is_jumping;
	
	// This is to determine if the
	// player is carrying a weapon
	bool m_carrying_weapon;

	// This is to determine if the
	// player is pushing a block
	bool m_is_pushing;

	// This is to set the jump height
	// from the player's current position
	float m_jump_height;

	// This determines the speed of
	// the player's jump
	float m_jump_speed;

public:

	// Constuctor and Destructor
	Player(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Player();

	// Function to set the jump height
	void SetJumpHeight(float value);

	// Functions to set and get the state
	// of jumping
	void SetJumpState(bool state);
	bool GetJumpState();

	// Functions to set and get the state
	// of carrying a weapon
	void SetWeaponCarryingState(bool state);
	bool GetWeaponCarryingState();

	// Functions to set and get the state
	// of pushing a block
	void SetPushState(bool state);
	bool GetPushState();

	// Functions to set and get the player's
	// movement speed
	void SetPlayerMoveSpeed(float speed);
	float GetPlayerMoveSpeed();

	// Functions to set and get the speed
	// multiplier
	void SetSpeedMultiplier(float amount);
	float GetSpeedMultiplier();

	// Functions to set, reset and get
	// the player's health
	void SetPlayerHealth(float amount);
	void ResetPlayerHealth();

	float GetPlayerHealth();
	float GetMaxHealth();

	// Functions to get and set the
	// player's jump height
	float GetJumpSpeed();
	float GetJumpHeight();


};
