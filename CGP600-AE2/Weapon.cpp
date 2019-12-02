#include "Weapon.h"

// Constructor
Weapon::Weapon(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char* assetFile, char* textureFile, float x_scale, float y_scale, float z_scale)
{
	// Initialising the asset
	InitialiseAsset(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);
	
	m_damage = 10.0f;

	// Initialising the booleans
	m_weapon_equipped = false;
	m_attacked = false;
	m_attack_complete = false;
	m_weapon_hit = false;

}

// Destructor
Weapon::~Weapon()
{

}

// SET AND GET FUNCTIONS OF ALL BOOLEANS

void Weapon::SetWeaponEquipState(bool state)
{
	m_weapon_equipped = state;
}

void Weapon::SetWeaponAttackedState(bool state)
{
	m_attacked = state;
}

void Weapon::SetWeaponAttackCompleteState(bool state)
{
	m_attack_complete = state;
}

void Weapon::SetWeaponHitState(bool state)
{
	m_weapon_hit = state;
}

float Weapon::GetWeaponDamage()
{
	return m_damage;
}

bool Weapon::GetWeaponEquipState()
{
	return m_weapon_equipped;
}

bool Weapon::GetWeaponAttackedState()
{
	return m_attacked;
}

bool Weapon::GetWeaponAttackCompleteState()
{
	return m_attack_complete;
}

bool Weapon::GetWeaponHitState()
{
	return m_weapon_hit;
}