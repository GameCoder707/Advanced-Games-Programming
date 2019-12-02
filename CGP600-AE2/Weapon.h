#pragma once

#include "Asset.h"

class Weapon : public Asset
{
private:

	float m_damage;

	// A boolean to check if the weapon
	// is equipped by a node
	bool m_weapon_equipped;
	
	// A boolean to check if it's in
	// attack state
	bool m_attacked;

	// A boolean to check if the attack
	// is completed
	bool m_attack_complete;

	// A boolean to check if the weapon had
	// hit something
	bool m_weapon_hit;


public:

	// Constructor and Destructor
	Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale);
	~Weapon();

	// SET AND GET FUNCTIONS FOR ALL BOOLEANS

	float GetWeaponDamage();

	void SetWeaponEquipState(bool state);
	bool GetWeaponEquipState();

	void SetWeaponHitState(bool state);
	bool GetWeaponHitState();

	void SetWeaponAttackedState(bool state);
	bool GetWeaponAttackedState();

	void SetWeaponAttackCompleteState(bool state);
	bool GetWeaponAttackCompleteState();

	
	

};
