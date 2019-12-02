#pragma once

// Including the header file to use the model object
#include "objfilemodel.h"

class Collider3D
{
private:

	// Collider's position values
	float m_x, m_y, m_z;

	// Collider's length, height and height
	float m_length, m_height, m_breadth;

	// Collider's radius
	float m_sqrt_radius;

public:

	// Collider class' constructor and destructor
	Collider3D();
	~Collider3D();

	// Function to calculate the centre of the collider
	void CalculateColliderCentre(ObjFileModel* model);

	// Function to calculate the length, 
	// height, and breadth of the model
	void CalculateDimensions(ObjFileModel* model);

	// Function to calculate the radius of the collider
	void CalculateRadius(ObjFileModel* model);


	// Self-Explanatory
	float GetLength(float x_scale);
	float GetHeight(float y_scale);
	float GetBreadth(float z_scale);

	float GetColliderRadius();

	XMVECTOR GetColliderPos();
	

};

