#include "SceneNode.h"

// This is the number of patrol
// spots the entity can travel to
const int patrol_spots = 4;

// Constructor
SceneNode::SceneNode(ID3D11Device* D3DDevice, ID3D11DeviceContext* ImmediateContext, char c, char* assetFile, char* textureFile, float x_pos, float y_pos, float z_pos, float x_scale, float y_scale, float z_scale, int gravityState)
{
	// Depending on the type of node, the
	// asset object is initialised

	// P = Player
	// E = Enemy
	// W = Weapon
	// S = Statik
	// B = Block
	// F = Food

	if (c == 'P')
	{
		m_p_asset = new Player(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale,  y_scale, z_scale);
	}
	else if (c == 'E')
	{
		m_e_asset = new Enemy(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale,  z_scale);
	}
	else if (c == 'W')
	{
		m_w_asset = new Weapon(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale,  y_scale, z_scale);
	}
	else if (c == 'S')
	{
		m_s_asset = new Statik(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale,  y_scale, z_scale);
	}
	else if (c == 'B')
	{
		m_b_asset = new Block(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale,  y_scale, z_scale);
	}
	else if (c == 'F')
	{
		m_f_asset = new Food(D3DDevice, ImmediateContext, assetFile, textureFile, x_scale, y_scale, z_scale);
	}

	// Initialising the random number generator
	srand(time(NULL));
	
	// Initialising the positions
	m_pos_x = x_pos;
	m_pos_y = y_pos;
	m_pos_z = z_pos;

	// Initialising the initial positions
	m_init_x = m_pos_x;
	m_init_y = m_pos_y;
	m_init_z = m_pos_z;

	// Initialising the angles
	m_xangle = 0.0f;
	m_yangle = 0.0f;
	m_zangle = 0.0f;

	m_prev_xangle = m_xangle;

	// Initialising the scale
	m_scale_x = x_scale;
	m_scale_y = y_scale;
	m_scale_z = z_scale;

	// Initialising the collider positions
	m_collider_centre_x = x_pos;
	m_collider_centre_y = y_pos;
	m_collider_centre_z = z_pos;

	// Initialising the look At distances
	m_lookAt_dist_x = 0.0f;
	m_lookAt_dist_z = 0.0f;

	// Initialising the range distances
	m_range_dist_x = 0.0f;
	m_range_dist_z = 0.0f;

	// Initialising the gravity speed
	m_gravity_speed = 4.0f;
		
	// Intialising the booleans
	m_is_colliding = false;
	m_is_interacting = false;
	m_in_range = false;
	m_halt_movement = false;
	m_on_ground = false;

	// Initialising the gravity state from
	// the parameter
	m_gravity_applied = gravityState;
	

}

// Destructor
SceneNode::~SceneNode()
{
	
}

void SceneNode::InitialisePatrolSpots()
{
	// Initialising the move spots for the entity to move towards
	for (int i = 0; i < patrol_spots; i++)
	{
		m_move_spots.push_back(XMVectorSet(m_pos_x + RandomBetweenFloats(-20, 20), m_pos_y, m_pos_z + RandomBetweenFloats(-20, 20), 0.0f));
	}

	// Initialising the random spots
	spot_num = GetRandomOf(patrol_spots);
	prev_spot_num = GetRandomOf(patrol_spots);

	// This is to ensure the previous and
	// current spot numbers are not equal
	if (prev_spot_num == spot_num)
	{
		prev_spot_num += 1;

		if (prev_spot_num > patrol_spots - 1)
		{
			prev_spot_num = 0;
		}
	}
}

// This calls the release all function for each
// asset object
void SceneNode::ReleaseAll()
{
	if (m_p_asset) m_p_asset->ReleaseAll();
	else if (m_e_asset) m_e_asset->ReleaseAll();
	else if (m_w_asset) m_w_asset->ReleaseAll();
	else if (m_s_asset) m_s_asset->ReleaseAll();
	else if (m_b_asset) m_b_asset->ReleaseAll();
	else if (m_f_asset) m_f_asset->ReleaseAll();

	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->ReleaseAll();
	}

}


void SceneNode::AddChildNode(SceneNode* n)
{
	// A child is added by pushing it
	// to the children vector
	m_children.push_back(n);
}

bool SceneNode::DetachNode(SceneNode* n)
{
	// Traverse tree to find node to detach

	// This loops through each child and erases
	// the child if it matches the node in the parameter
	for (int i = 0; i < m_children.size(); i++)
	{
		if (n == m_children[i])
		{
			m_children.erase(m_children.begin() + i);
			return true;
		}

		if (m_children[i]->DetachNode(n) == true) return true;
	}

	return false;
}

void SceneNode::Draw(XMMATRIX* world, XMMATRIX* view, XMMATRIX* projection)
{
	
	// We call the draw function if the object is active
	if      (m_p_asset) m_p_asset->Draw(&GetWorldMatrix(world), view, projection);
	else if (m_e_asset) m_e_asset->Draw(&GetWorldMatrix(world), view, projection);
	else if (m_w_asset) m_w_asset->Draw(&GetWorldMatrix(world), view, projection);
	else if (m_s_asset) m_s_asset->Draw(&GetWorldMatrix(world), view, projection);
	else if (m_b_asset) m_b_asset->Draw(&GetWorldMatrix(world), view, projection);
	else if (m_f_asset) m_f_asset->Draw(&GetWorldMatrix(world), view, projection);

	// We execute this function for the node's children
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->Draw(&GetWorldMatrix(world), view, projection);
	}


}

XMMATRIX SceneNode::GetWorldMatrix(XMMATRIX* world)
{
	// Initialising a world matrix
	XMMATRIX world_matrix = XMMatrixIdentity();

	// Multiplying the world matrix with scaling, rotation and translation
	// matrices
	world_matrix *= XMMatrixScaling(m_scale_x, m_scale_y, m_scale_z);
	world_matrix *= XMMatrixRotationRollPitchYaw(m_xangle, m_yangle, m_zangle);
	world_matrix *= XMMatrixTranslation(m_pos_x, m_pos_y, m_pos_z);

	// Finally, we multiply the world matrix with the parent's
	// world matrix
	world_matrix *= *world;

	return world_matrix;

}

void SceneNode::UpdateCollisionTree(XMMATRIX* world)
{

	XMVECTOR v;

	// If the asset is active, we get its collider
	// positions
	if (m_p_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_p_asset->collider->GetColliderPos()), XMVectorGetY(m_p_asset->collider->GetColliderPos()), XMVectorGetZ(m_p_asset->collider->GetColliderPos()), 0.0f);
	}
	else if (m_w_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_w_asset->collider->GetColliderPos()), XMVectorGetY(m_w_asset->collider->GetColliderPos()), XMVectorGetZ(m_w_asset->collider->GetColliderPos()), 0.0f);
	}
	else if (m_e_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_e_asset->collider->GetColliderPos()), XMVectorGetY(m_e_asset->collider->GetColliderPos()), XMVectorGetZ(m_e_asset->collider->GetColliderPos()), 0.0f);
	}
	else if (m_s_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_s_asset->collider->GetColliderPos()), XMVectorGetY(m_s_asset->collider->GetColliderPos()), XMVectorGetZ(m_s_asset->collider->GetColliderPos()), 0.0f);
	}
	else if (m_b_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_b_asset->collider->GetColliderPos()), XMVectorGetY(m_b_asset->collider->GetColliderPos()), XMVectorGetZ(m_b_asset->collider->GetColliderPos()), 0.0f);
	}
	else if (m_f_asset)
	{
		v = XMVectorSet(XMVectorGetX(m_f_asset->collider->GetColliderPos()), XMVectorGetY(m_f_asset->collider->GetColliderPos()), XMVectorGetZ(m_f_asset->collider->GetColliderPos()), 0.0f);
	}
	else
	{
		v = XMVectorSet(0.0, 0.0, 0.0, 0.0);
	}

	// Transforming the vector to the world matrix
	v = XMVector3Transform(v, GetWorldMatrix(world));

	// The updated collider positions are set
	// from the vector
	m_collider_centre_x = XMVectorGetX(v);
	m_collider_centre_y = XMVectorGetY(v);
	m_collider_centre_z = XMVectorGetZ(v);

	// This function is executed for the node's children
	for (int i = 0; i < m_children.size(); i++)
	{
		m_children[i]->UpdateCollisionTree(&GetWorldMatrix(world));
	}
}

void SceneNode::CalculateBoxDimensions1(XMVECTOR v, Asset* obj)
{

	// We're trying to get the front top-left position of the asset
	x1 = XMVectorGetX(v) - (obj->collider->GetLength(obj->GetXScale()) / 2);
	y1 = XMVectorGetY(v) + (obj->collider->GetHeight(obj->GetYScale()) / 2);
	z1 = XMVectorGetZ(v) - (obj->collider->GetBreadth(obj->GetZScale()) / 2);

	// Retrieving length, height and breadth values
	// from the asset's collider
	l1 = obj->collider->GetLength(obj->GetXScale());
	h1 = obj->collider->GetHeight(obj->GetYScale());
	b1 = obj->collider->GetBreadth(obj->GetZScale());

}

void SceneNode::CalculateSphereDimensions1(XMVECTOR v, Asset* obj)
{
	// We simply get the centre position of
	// the asset and the radius from the collider
	x1 = XMVectorGetX(v);
	y1 = XMVectorGetY(v);
	z1 = XMVectorGetZ(v);

	r1 = obj->collider->GetColliderRadius();
}

void SceneNode::CalculateBoxDimensions2(XMVECTOR v, Asset* obj)
{
	x2 = XMVectorGetX(v) - (obj->collider->GetLength(obj->GetXScale()) / 2);
	y2 = XMVectorGetY(v) + (obj->collider->GetHeight(obj->GetYScale()) / 2);
	z2 = XMVectorGetZ(v) - (obj->collider->GetBreadth(obj->GetZScale()) / 2);

	l2 = obj->collider->GetLength(obj->GetXScale());
	h2 = obj->collider->GetHeight(obj->GetYScale());
	b2 = obj->collider->GetBreadth(obj->GetZScale());

}

void SceneNode::CalculateSphereDimensions2(XMVECTOR v, Asset* obj)
{
	x2 = XMVectorGetX(v);
	y2 = XMVectorGetY(v);
	z2 = XMVectorGetZ(v);

	r2 = obj->collider->GetColliderRadius();
}

void SceneNode::CalculateBoxCollisionDetails(SceneNode* compare_tree)
{
	// Retrieving the collider's center positions
	XMVECTOR v1 = GetWorldColliderCentrePos();
	XMVECTOR v2 = compare_tree->GetWorldColliderCentrePos();

	// We calculate the box dimensions for which
	// asset is active
	if (m_p_asset)
	{
		CalculateBoxDimensions1(v1, m_p_asset);
	}
	else if (m_e_asset)
	{
		CalculateBoxDimensions1(v1, m_e_asset);
	}
	else if (m_w_asset)
	{
		CalculateBoxDimensions1(v1, m_w_asset);
	}
	else if (m_s_asset)
	{
		CalculateBoxDimensions1(v1, m_s_asset);
	}
	else if (m_b_asset)
	{
		CalculateBoxDimensions1(v1, m_b_asset);
	}
	else if (m_f_asset)
	{
		CalculateBoxDimensions1(v1, m_f_asset);
	}

	// This is for the other asset
	if (compare_tree->m_p_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_p_asset);
	}
	else if (compare_tree->m_e_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_e_asset);
	}
	else if (compare_tree->m_w_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_w_asset);
	}
	else if (compare_tree->m_s_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_s_asset);
	}
	else if (compare_tree->m_b_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_b_asset);
	}
	else if (compare_tree->m_f_asset)
	{
		CalculateBoxDimensions2(v2, compare_tree->m_f_asset);
	}

}

void SceneNode::CalculateSphereCollisionDetails(SceneNode* compare_tree)
{

	// We do the same as CalculateBoxCollisionDetails

	XMVECTOR v1 = GetWorldColliderCentrePos();
	XMVECTOR v2 = compare_tree->GetWorldColliderCentrePos();

	if (m_p_asset)
	{
		CalculateSphereDimensions1(v1, m_p_asset);
	}
	else if (m_e_asset)
	{
		CalculateSphereDimensions1(v1, m_e_asset);
	}
	else if (m_w_asset)
	{
		CalculateSphereDimensions1(v1, m_w_asset);
	}
	else if (m_s_asset)
	{
		CalculateSphereDimensions1(v1, m_s_asset);
	}
	else if (m_b_asset)
	{
		CalculateSphereDimensions1(v1, m_b_asset);
	}
	else if (m_f_asset)
	{
		CalculateSphereDimensions1(v1, m_f_asset);
	}


	if (compare_tree->m_p_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_p_asset);
	}
	else if (compare_tree->m_e_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_e_asset);
	}
	else if (compare_tree->m_w_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_w_asset);
	}
	else if (compare_tree->m_s_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_s_asset);
	}
	else if (compare_tree->m_b_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_b_asset);
	}
	else if (compare_tree->m_f_asset)
	{
		CalculateSphereDimensions2(v2, compare_tree->m_f_asset);
	}

	// Calculating distance between objects
	m_dist_x = x2 - x1;
	m_dist_y = y2 - y1;
	m_dist_z = z2 - z1;

	// Using Pythagaros to calculate the actual distance
	main_dist = sqrt((m_dist_x * m_dist_x) + (m_dist_y * m_dist_y) + (m_dist_z * m_dist_z));

	// Calculating the sum of radii
	sum_radius = r1 + r2;


}

bool SceneNode::CheckCollision(SceneNode* compare_tree)
{
	// This is to avoid collision checks
	// against itself
	if (this == compare_tree)
	{
		return false;
	}

	// We need details for Box collision so we call this
	CalculateBoxCollisionDetails(compare_tree);

	// If atleast one end of the object is
	// colliding with the end of the other
	// object, it returns true
	if (x1 < x2 + l2 && x1 + l1 > x2)
	{
		if (y1 > y2 - h2 && y1 - h1 < y2)
		{
			if (z1 < z2 + b2 && z1 + b1 > z2)
			{
				m_is_colliding = true;
				return true;
			}
			else
			{
				m_is_colliding = false;
				return false;
			}
		}
		else
		{
			m_is_colliding = false;
			return false;
		}
	}
	else
	{
		m_is_colliding = false;
		return false;
	}
	

}

bool SceneNode::CheckActionCollision(SceneNode* compare_tree)
{
	if (this == compare_tree)
	{
		return false;
	}

	// Retrieving the details for sphere collision checks
	CalculateSphereCollisionDetails(compare_tree);

	// If the distance between the objects is
	// less than the sum of radii, it returns 
	// true
	if (main_dist < sum_radius )
	{
		m_is_interacting = true;
		return true;
	}
	else
	{
		m_is_interacting = false;
		return false;
	}


}

bool SceneNode::CheckNodeBottomCollision(SceneNode* compare_tree)
{
	if (this == compare_tree)
	{
		return false;
	}

	// We again need box collision details for this
	CalculateBoxCollisionDetails(compare_tree);

	if (x1 < x2 + l2 && x1 + l1 > x2)
	{
		// we just check if the bottom is above the ground
		// but below a few units from the ground
		if ((y1 - h1 > y2 && y1 - h1 < y2 + 0.07))
		{
			if (z1 < z2 + b2 && z1 + b1 > z2)
			{
				m_on_ground = true;
				return true;
			}
			else
			{
				m_on_ground = false;
				return false;
			}
		}
		else
		{
			m_on_ground = false;
			return false;
		}
	}
	else
	{
		m_on_ground = false;
		return false;
	}


}


void SceneNode::MoveAsset(float x_dist, float y_dist, float z_dist)
{
	// If the asset isn't halted, move the asset
	if (m_halt_movement == false)
	{
		m_pos_x += x_dist;
		m_pos_y += y_dist;
		m_pos_z += z_dist;
	}
	
}

void SceneNode::ApplyGravity(double deltaTime)
{
	// If the asset isn't on ground, gravity is enabled
	// and deltaTime is less than 1 so that it doesn't
	// alter the values by a large amount
	if (m_on_ground == false && m_gravity_applied == true && deltaTime < 1)
	{
		// We move the asset downward by gravity
		MoveAsset(0.0f, -m_gravity_speed * deltaTime, 0.0f);
	}

}

void SceneNode::ResetToInitalPos()
{
	// Positions are reset to initial
	// pos
	m_pos_x = m_init_x;
	m_pos_y = m_init_y;
	m_pos_z = m_init_z;

	m_prev_x = m_init_x;
	m_prev_y = m_init_y;
	m_prev_z = m_init_z;

}

float SceneNode::GetRandomOf(int num)
{
	if (num > RAND_MAX)
	{
		num = RAND_MAX;
	}
	else if (num == 0)
	{
		num = 1;
	}

	// Returns a random between 0 and num
	// included
	return rand() % num;
}

void SceneNode::SetToPreviousSpot()
{
	// The current spot is set
	// to the previous spot
	spot_num = prev_spot_num;
}

void SceneNode::LookAt()
{
	// If the player is not in range
	// of the entity, this returns true
	if (m_in_range == false)
	{
		// We get the distance between the entity
		// and the spot's location
		m_lookAt_dist_x = m_pos_x - XMVectorGetX(m_move_spots[spot_num]);
		m_lookAt_dist_z = m_pos_z - XMVectorGetZ(m_move_spots[spot_num]);

		// If the entity reached the spot, this returns true
		if (fabs(m_lookAt_dist_x) <= 0.2f && fabs(m_lookAt_dist_z) <= 0.2f)
		{
			// We assign a random spot
			prev_spot_num = spot_num;
			spot_num = GetRandomOf(patrol_spots);

			// Just like the initiation stage,
			// we avoid both to be same
			if (spot_num == prev_spot_num)
			{
				spot_num += 1;

				if (spot_num > patrol_spots - 1)
				{
					spot_num = 0;
				}
			}


			m_lookAt_dist_x = m_pos_x - XMVectorGetX(m_move_spots[spot_num]);
			m_lookAt_dist_z = m_pos_z - XMVectorGetZ(m_move_spots[spot_num]);

		}
	}

	m_yangle = -atan2(m_lookAt_dist_x, -m_lookAt_dist_z);

}

void SceneNode::CheckInRange(XMVECTOR other_pos)
{
	// We get the distance between the entity
	// and the player
	m_range_dist_x = m_pos_x - XMVectorGetX(other_pos);
	m_range_dist_z = m_pos_z - XMVectorGetZ(other_pos);

	// If we are in range, this returns true
	if (fabs(m_range_dist_x) <= 10.0f && fabs(m_range_dist_z) <= 10.0f)
	{
		m_in_range = true;

		// If the entity has reached close enough, this will return true
		if (fabs(m_range_dist_x) <= 2.0f && fabs(m_range_dist_z) <= 2.0f)
		{
			m_halt_movement = true;
		}
		else
		{
			m_halt_movement = false;
		}
		
		// Now the look at direction is made
		// towards the player
		m_lookAt_dist_x = m_range_dist_x;
		m_lookAt_dist_z = m_range_dist_z;

	}
	else
	{
		m_in_range = false;
	}


}


void SceneNode::RotateAsset(float pitch_degrees, float yaw_degrees, float roll_degrees)
{
	// Rotating the asset with with parameter values

	m_xangle += XMConvertToRadians(pitch_degrees);
	m_yangle += XMConvertToRadians(yaw_degrees);
	m_zangle += XMConvertToRadians(roll_degrees);

}

void SceneNode::RestrictPos(bool isColliding)
{
	// If colliding, set current pos
	// to previous pos
	if (isColliding == true)
	{
		m_pos_x = m_prev_x;
		m_pos_y = m_prev_y;
		m_pos_z = m_prev_z;
	}
}

void SceneNode::UpdatePos(bool isColliding)
{
	// If not colliding, update
	// previous positions
	if (isColliding == false)
	{
		m_prev_x = m_pos_x;
		m_prev_y = m_pos_y;
		m_prev_z = m_pos_z;
	}

}

void SceneNode::SetXPos(float x)
{
	m_pos_x = x;
}

void SceneNode::SetYPos(float y)
{
	m_pos_y = y;
}

void SceneNode::SetZPos(float z)
{
	m_pos_z = z;
}

void SceneNode::SetYAngle(float angle)
{
	m_yangle = angle;
}

void SceneNode::RestrictPitch()
{
	// 0 is at the center
	// So, we restrict it between -90 and +90 degrees by
	// setting it to the previous angle
	if (m_xangle > (3.14f / 2) || m_xangle < -(3.14f / 2))
	{
		m_xangle = m_prev_xangle;
	}
	else
	{
		m_prev_xangle = m_xangle;
	}
}

void SceneNode::SetCurZPos()
{
	m_cur_pos_z = m_pos_z;
}

void SceneNode::SetCollideState(bool state)
{
	m_is_colliding = state;
}

bool SceneNode::IsColliding()
{
	return m_is_colliding;
}

void SceneNode::SetInteractState(bool state)
{
	m_is_interacting = state;
}

bool SceneNode::IsInteracting()
{
	return m_is_interacting;
}

void SceneNode::SetOnGroundState(bool state)
{
	m_on_ground = state;
}

bool SceneNode::GetOnGroundState()
{
	return m_on_ground;
}

void SceneNode::SetInRangeState(bool state)
{
	m_in_range = state;
}

bool SceneNode::GetInRangeState()
{
	return m_in_range;
}

void SceneNode::SetHaltState(bool state)
{
	m_halt_movement = state;
}

bool SceneNode::isHalted()
{
	return m_halt_movement;
}

int SceneNode::GetChildrenSize()
{
	return m_children.size();
}

vector<SceneNode*> SceneNode::GetChildren()
{
	return m_children;
}

XMVECTOR SceneNode::GetWorldColliderCentrePos()
{
	return XMVectorSet(m_collider_centre_x, m_collider_centre_y, m_collider_centre_z, 0.0f);
}

XMVECTOR SceneNode::GetWorldPos()
{
	return XMVectorSet(m_pos_x, m_pos_y, m_pos_z, 0.0f);
}

SceneNode* SceneNode::GetEquippedWeaponNode()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		// If the node has a valid weapon asset,
		// we return that child node
		if (m_children[i]->m_w_asset)
		{
			return m_children[i];
		}
	}
}

SceneNode* SceneNode::GetPushingCrate()
{
	for (int i = 0; i < m_children.size(); i++)
	{
		// If the node has a valid weapon asset,
		// we return that child node
		if (m_children[i]->m_b_asset)
		{
			return m_children[i];
		}
	}
}

// Random Function
float SceneNode::RandomBetweenFloats(float min, float max)
{
	// Calculating a random value below 1
	float random = float(rand()) / float(RAND_MAX);

	// Retrieving the difference of max and min
	float diff = max - min;

	// Getting a value to add up to the minimum
	float randValue = random * diff;

	return min + randValue;

}

// GET FUNCTIONS

float SceneNode::GetXPos()
{
	return m_pos_x;
}

float SceneNode::GetYPos()
{
	return m_pos_y;
}

float SceneNode::GetZPos()
{
	return m_pos_z;
}

float SceneNode::GetCurZPos()
{
	return m_cur_pos_z;
}

float SceneNode::GetXAngle()
{
	return m_xangle;
}

float SceneNode::GetYAngle()
{
	return m_yangle;
}

float SceneNode::GetZAngle()
{
	return m_zangle;
}

float SceneNode::GetXScale()
{
	return m_scale_x;
}

float SceneNode::GetYScale()
{
	return m_scale_y;
}

float SceneNode::GetZScale()
{
	return m_scale_z;
}

float SceneNode::GetGravitySpeed()
{
	return m_gravity_speed;
}

