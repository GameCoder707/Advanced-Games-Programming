#pragma once

// Definitions for XNAMATH to
// work efficiently
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <d3dx11.h>
#include <dxerr.h>
#include <xnamath.h>

class Light
{
private:

	// Vector to project the light from a position
	XMVECTOR m_directional_light_shines_from;

	// Vector to determine the colour of the light
	XMVECTOR m_directional_light_colour;

	// Vector to determine the ambient
	// light colour. If there is no light,
	// this will illuminate the world
	XMVECTOR m_ambient_light_colour;

public:

	// Constructor and Destructor
	Light();
	~Light();

	// Set functions for the vectors declared above
	void SetDirectionalLightPos(float x, float y, float z, float w);
	void SetDirectionalLightColour(float r, float g, float b, float a);
	void SetAmbientLightColour(float r, float g, float b, float a);

	// Get functions for the vectors declared above
	XMVECTOR GetDirectionalLightPos();
	XMVECTOR GetDirectionalLightColour();
	XMVECTOR GetAmbientLightColour();

};
