#pragma once

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3d11.h>
#include <xnamath.h>
#include <time.h>

// Including children of Asset class
// to use them as nodes
#include "Player.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Statik.h"
#include "Block.h"
#include "Food.h"

class SceneNode
{
private:

	// Vector of children nodes
	vector<SceneNode*> m_children;

	// Position of nodes
	float m_pos_x, m_pos_y, m_pos_z;

	// Initial Position of Nodes
	float m_init_x, m_init_y, m_init_z;

	// Current Z position of the node
	// (Used by weapons for combat)
	float m_cur_pos_z;

	// Previous positions of the node
	float m_prev_x, m_prev_y, m_prev_z;

	// Degree values for rotating nodes
	float m_xangle, m_yangle, m_zangle;

	// To store previous pitch angle
	// to restrict them between limits
	float m_prev_xangle;

	// This is to store the scale of the node
	float m_scale_x, m_scale_y, m_scale_z;

	// These values are used for box collision
	// calculations. More info can be 
	// seen in calculation code
	float x1, y1, z1, r1;
	float l1, h1, b1;

	float x2, y2, z2, r2;
	float l2, h2, b2;

	// This is to store the distance
	// between 2 objects
	float m_dist_x;
	float m_dist_y;
	float m_dist_z;

	// This is the distance between an entity
	// and a point or object and to check
	// if they arrived at their destination
	// or not
	float m_lookAt_dist_x;
	float m_lookAt_dist_z;

	// This is the distance between the
	// player and an entity
	float m_range_dist_x;
	float m_range_dist_z;

	// This is the power of the gravitional
	// pull applied to objects
	float m_gravity_speed;	

	// This to determine which spot
	// the entity should move towards
	int spot_num;
	int prev_spot_num;

	// This is to store the final radius
	// after calculations
	float main_dist;

	// This is to store the sum or radii
	// for sphere collision checks
	float sum_radius;

	// These are to store the values
	// from the colliders of asset's
	// children classes
	float m_collider_centre_x;
	float m_collider_centre_y;
	float m_collider_centre_z;

	// This is to determine if the object
	// is colliding against something
	bool m_is_colliding;

	// This is to determine if the object is
	// interacting with something
	bool m_is_interacting;

	// This is to determine if the node is
	// on top of something
	bool m_on_ground;

	// This is to determine if gravity is
	// applied to that object
	int m_gravity_applied;

	// This is to determine if the entity
	// is in range of the player
	bool m_in_range;

	// This is to halt the movement of the entity
	// if it had approached the player
	bool m_halt_movement;

	// This stores the postions
	// for the entity to move towards
	// randomly
	vector<XMVECTOR> m_move_spots;

public:

	// Declaring Objects
	Player* m_p_asset;
	Enemy*  m_e_asset;
	Weapon* m_w_asset;
	Statik* m_s_asset;
	Block* m_b_asset;
	Food* m_f_asset;


	// Constructor and Destructor
	SceneNode(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char c, char* assetFile, char* textureFile, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale, int gravityState);
	~SceneNode();

	void InitialisePatrolSpots();

	// This is to release all the DirectX 11
	// objects of the assets
	void ReleaseAll();

	// This adds a child node
	void AddChildNode(SceneNode* n);

	// This removes a child node
	bool DetachNode(SceneNode* n);

	// This function executes the draw function of the nodes
	// and its children
	void Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection);

	// This calculates and returns the world matrix
	XMMATRIX GetWorldMatrix(XMMATRIX* world);

	// This updates the position of the node using
	// the asset's collider's values
	void UpdateCollisionTree(XMMATRIX* world);

	// This checks collision of one object
	// against another
	bool CheckCollision(SceneNode* compare_tree);

	// This checks if an object is interacting
	// with another
	bool CheckActionCollision(SceneNode* compare_tree);

	// This checks if an object is colliding with
	// something below
	bool CheckNodeBottomCollision(SceneNode* compare_tree);

	// This calculates collision for spheres
	void CalculateSphereCollisionDetails(SceneNode* compare_tree);

	// This calculates collision for boxes
	void CalculateBoxCollisionDetails(SceneNode* compare_tree);

	// These receive data needed for box collision checks
	void CalculateBoxDimensions1(XMVECTOR v, Asset* obj);
	void CalculateSphereDimensions1(XMVECTOR v, Asset* obj);

	// These receive data needed for sphere collision checks
	void CalculateBoxDimensions2(XMVECTOR v, Asset* obj);
	void CalculateSphereDimensions2(XMVECTOR v, Asset* obj);

	// This applies gravity to objects that
	// has gravity enabled
	void ApplyGravity(double deltaTime);

	// This returns position of the collider
	XMVECTOR GetWorldColliderCentrePos();

	// This returns position of the node
	XMVECTOR GetWorldPos();

	// These set positions for the node
	void SetXPos(float x);
	void SetYPos(float y);
	void SetZPos(float z);

	// This retricts the pitch of the
	// node
	void RestrictPitch();

	// This sets the yaw angle
	void SetYAngle(float angle);

	// This makes the entity look at
	// something
	void LookAt();
	
	// This checks if the player is in
	// range of the entity
	void CheckInRange(XMVECTOR other_pos);

	// This sets and gets the
	// current of Z Pos
	void SetCurZPos();
	float GetCurZPos();

	// Returns the position of each axis
	float GetXPos();
	float GetYPos();
	float GetZPos();

	// Returns the angle of each axis
	float GetXAngle();
	float GetYAngle();
	float GetZAngle();

	// This returns the scale of each axis
	float GetXScale();
	float GetYScale();
	float GetZScale();

	// This returns the gravity speed
	float GetGravitySpeed();

	// This restricts the position of the node
	// if it's colliding
	void RestrictPos(bool isCollding);

	// This updates the position of the node
	// if it's not colliding
	void UpdatePos(bool isColliding);

	// This moves the asset in all directions
	void MoveAsset(float x_dist, float y_dist, float z_dist);

	// This rotates the asset in all directions
	void RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees);

	// This resets to the asset's initial position
	void ResetToInitalPos();

	// A function to return a random value
	// between the min and max
	float RandomBetweenFloats(float min, float max);

	// This returns a random spot
	// for the entity to move towards
	float GetRandomOf(int num);

	// This sets the current spot
	// to its previous spot
	void SetToPreviousSpot();

	// Set and Get the colliding state
	void SetCollideState(bool state);
	bool IsColliding();

	// Set and Get the interacting state
	void SetInteractState(bool state);
	bool IsInteracting();

	// Set and Get the on ground state
	void SetOnGroundState(bool state);
	bool GetOnGroundState();

	// Set and Get in Range state
	void SetInRangeState(bool state);
	bool GetInRangeState();

	// Set and Get Is Halted state
	void SetHaltState(bool state);
	bool isHalted();

	// Returns the size of children
	int GetChildrenSize();


	// This returns the node's children nodes
	vector<SceneNode*> GetChildren();

	// Returns the weapon node equipped 
	// by its parent node
	SceneNode* GetEquippedWeaponNode();

	// Returns the crate node pushed 
	// by its parent node
	SceneNode* GetPushingCrate();

};