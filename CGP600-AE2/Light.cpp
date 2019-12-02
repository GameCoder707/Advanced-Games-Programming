#include "Light.h"

// Constructor
Light::Light()
{
	// Initialising the default values for the vectors
	m_directional_light_shines_from = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_directional_light_colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	m_ambient_light_colour = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
}

Light::~Light()
{

}

// The vectors are set from the parameter values
void Light::SetDirectionalLightPos(float x, float y, float z, float w)
{
	m_directional_light_shines_from = XMVectorSet(x, y, z, w);
}

void Light::SetDirectionalLightColour(float r, float g, float b, float a)
{
	m_directional_light_colour = XMVectorSet(r, g, b, a);
}

void Light::SetAmbientLightColour(float r, float g, float b, float a)
{
	m_ambient_light_colour = XMVectorSet(r, g, b, a);
}

// These functions return the vectors respectively
XMVECTOR Light::GetDirectionalLightPos()
{
	return m_directional_light_shines_from;
}

XMVECTOR Light::GetDirectionalLightColour()
{
	return m_directional_light_colour;
}

XMVECTOR Light::GetAmbientLightColour()
{
	return m_ambient_light_colour;
}