#include "Collider3D.h"

// Constructor
Collider3D::Collider3D()
{

	// Initialising the position
	// and scale values
	m_x = 0;
	m_y = 0;
	m_z = 0;

	m_length  = 0;
	m_breadth = 0;
	m_height  = 0;

}

Collider3D::~Collider3D()
{

}

void Collider3D::CalculateColliderCentre(ObjFileModel* model)
{
	// Temporary variables are declared and initialised
	// to store the minimum and maximum XYZ positions
	float min_x = 0;
	float min_y = 0;
	float min_z = 0;

	float max_x = 0;
	float max_y = 0;
	float max_z = 0;

	// We are now going through each vertex of the model
	for (int i = 0; i < model->numverts; i++)
	{
		// If the x-position of the point is 
		// less than the minimum x, it returns true
		if (model->vertices[i].Pos.x < min_x)
		{
			min_x = model->vertices[i].Pos.x;
		} 
		// Else if the x-position is greater than
		// the maximum x-position, it return  true
		else if (model->vertices[i].Pos.x > max_x)
		{
			max_x = model->vertices[i].Pos.x;
		}

		// Same process for y and z positions

		if (model->vertices[i].Pos.y < min_y)
		{
			min_y = model->vertices[i].Pos.y;
		}
		else if (model->vertices[i].Pos.y > max_y)
		{
			max_y = model->vertices[i].Pos.y;
		}

		if (model->vertices[i].Pos.z < min_z)
		{
			min_z = model->vertices[i].Pos.z;
		}
		else if (model->vertices[i].Pos.z > max_z)
		{
			max_z = model->vertices[i].Pos.z;
		}
	}

	// Minimum X/Y/Z position is the lowest of its respective axis
	// Maximum X/Y/Z position is the highest of its respective axis

	// By subtracting these two values, we get the distance between them
	// We then divide by 2 to get half the distance
	// Finally, we add the minimum to the halved distance to get the
	// center position of the collider

	m_x = min_x + ((max_x - min_x) / 2);
	m_y = min_y + ((max_y - min_y) / 2);
	m_z = min_z + ((max_z - min_z) / 2);

}

void Collider3D::CalculateDimensions(ObjFileModel* model)
{
	// Temporary values to store the distance between
	// the center point and any point of the model
	float dist_x = 0.0f;
	float dist_y = 0.0f;
	float dist_z = 0.0f;

	// We now loop through each vertex of the model
	for (int i = 0; i < model->numverts; i++)
	{
		// We want the largest distance. So we now check the
		// distance between a point of the model and center point and 
		// check if it's greater than the previous point
		if (model->vertices[i].Pos.x - m_x > dist_x)
		{
			dist_x = model->vertices[i].Pos.x - m_x;
		}

		if (model->vertices[i].Pos.y - m_y > dist_y)
		{
			dist_y = model->vertices[i].Pos.y - m_y;
		}

		if (model->vertices[i].Pos.z - m_z > dist_z)
		{
		dist_z = model->vertices[i].Pos.z - m_z;
		}

	}		

	// Length  represents the dimension in x-axis
	// Height  represents the dimension in y-axis
	// Breadth represents the dimension in z-axis

	// We only calulcated the distance between the center point
	// and the farthest pointo of the model. So we multiply by 2 to get
	// length, height and breadth
	m_length = dist_x * 2;
	m_height = dist_y * 2;
	m_breadth = dist_z * 2;
}

void Collider3D::CalculateRadius(ObjFileModel* model)
{
	// Temporary variables to calculate various distances
	float dist_x = 0.0f;
	float dist_y = 0.0f;
	float dist_z = 0.0f;
	float cal_dist = 0.0f;
	float max_dist = 0.0f;

	// We now loop through each vertex of the model
	for (int i = 0; i < model->numverts; i++)
	{
		
		// We are calculating the distances in all
		// 3 axes
		dist_x = model->vertices[i].Pos.x - m_x;
		
		dist_y = model->vertices[i].Pos.y - m_y;
		
		dist_z = model->vertices[i].Pos.z - m_z;
		
		// We now apply the Pythagoras Theorm to calculate the squared radius
		// It is done this way to improve performance
		cal_dist = (dist_x * dist_x) + (dist_y * dist_y) + (dist_z * dist_z);

		// If the calculated distance is greater than the maximum
		// distance, then update the maximum distance
		if (cal_dist > max_dist)
		{
			max_dist = cal_dist;
		}

	}

	// We then apply the square root to get
	// the real radius value
	m_sqrt_radius = sqrt(max_dist);	

}

XMVECTOR Collider3D::GetColliderPos()
{
	// We set the position values in a vector to return
	// it as a position vector
	XMVECTOR pos = XMVectorSet(m_x, m_y, m_z, 0.0f);
	
	return pos;

}

// We get the length, height and breadth by multiplying
// the scale of the respective axis
float Collider3D::GetLength(float x_scale)
{
	return m_length * x_scale;
}

float Collider3D::GetHeight(float y_scale)
{
	return m_height * y_scale;
}

float Collider3D::GetBreadth(float z_scale)
{
	return m_breadth * z_scale;
}

// Returns radius value
float Collider3D::GetColliderRadius()
{
	return m_sqrt_radius;
}