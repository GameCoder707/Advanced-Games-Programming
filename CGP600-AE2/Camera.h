#pragma once

// Definitions to function XNAMATH properly
#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

// Including the DirectX 11 header file
// and maths header files
#include <d3d11.h>
#include <xnamath.h>
#include <math.h>

class Camera
{
private:

	// Camera's position values
	float m_x, m_y, m_z;
	
	// Camera's rotation values
	float m_dx, m_dy, m_dz;

	// Camera's rotational degrees
	// on both the axes
	float m_camera_rotation_x;
	float m_camera_rotation_y;

	// To store the previous Y-axis
	// rotation
	float m_prev_rot_y;

	// This is for the skybox
	float near_clip_plane;

	// Vectors used to get the view matrix
	XMVECTOR m_position, m_lookAt, m_right, m_up, m_forward;

	// Matrix to store the camera's projection values
	XMMATRIX projection;

public:

	// Camera class constructor
	Camera(float x, float y, float z, float camera_rotation, float FOV, float winWidth, float winHeight, float nearClip, float farClip);

	// Function to move the camera independently
	void Move(float x_distance, float y_distance, float z_distance);

	// Function to move the camera along with the player
	void MoveWithPlayer(float x, float y, float z);

	// Rotate the camera on both the axes
	void YawCamera(float amount);
	void PitchCamera(float amount);

	// Returns the position of the camera
	XMVECTOR GetPosition();

	XMFLOAT3 GetPos();

	float GetNearClipPlane();

	// Returns view and projection matrix
	// of the camera
	XMMATRIX GetViewMatrix();
	XMMATRIX GetProjectionMatrix();
};
