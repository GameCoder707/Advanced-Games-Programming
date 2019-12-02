#include "Camera.h"

Camera::Camera(float x, float y, float z, float camera_rotation, float FOV, float winWidth, float winHeight, float nearClip, float farClip)
{
	// Initialising the position values
	m_x = x;
	m_y = y;
	m_z = z;

	// Initialising the camera's rotational radians
	m_camera_rotation_x = XMConvertToRadians(camera_rotation);
	m_camera_rotation_y = XMConvertToRadians(camera_rotation);

	// Initialising the rotational values
	m_dx = sin(XMConvertToRadians(camera_rotation));
	m_dy = sin(XMConvertToRadians(camera_rotation));
	m_dz = cos(XMConvertToRadians(camera_rotation));

	// Initialising the previous pitch radian
	m_prev_rot_y = camera_rotation;

	// Initialising the up and position vectors
	m_right = XMVectorSet(1.0, 0.0, 0.0, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);
	m_forward = XMVectorSet(0.0, 0.0, 1.0, 0.0);
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0f);

	// Initialising the projection matrix from the Asset_Details script
	// FOV = Field of View
	// winWidth = Window Width
	// winHeight = Window Height
	// nearClip = Near Clipping Plane
	// farClip = Far Clipping Plane
	projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(FOV), winWidth / winHeight, nearClip, farClip);

	near_clip_plane = nearClip;

}

// Rotate the camera in yaw axis
void Camera::YawCamera(float amount)
{
	// Increasing the yaw radians
	m_camera_rotation_y += XMConvertToRadians(amount);

	// Updating rotational values by
	// calculating the sin and cos of the radians
	m_dx = sin(m_camera_rotation_y);
	m_dz = cos(m_camera_rotation_y);

}

// Rotate the camera in pitch axis
void Camera::PitchCamera(float amount)
{
	// Increasing the pitch radians
	m_camera_rotation_x += XMConvertToRadians(amount);

	// This is to restrict the camera's pitch radians within limits
	if (m_camera_rotation_x > (3.14f/2) || m_camera_rotation_x < -(3.14/2))
	{
		m_camera_rotation_x = m_prev_rot_y;
	}
	else
	{
		m_prev_rot_y = m_camera_rotation_x;
	}

	// Updating the rotational values
	m_dy = sin(m_camera_rotation_x);
	m_dz = cos(m_camera_rotation_x);


}

void Camera::Move(float x_distance, float y_distance, float z_distance)
{
	
	m_forward = XMVector3Normalize(m_lookAt - m_position);
	m_right = XMVector3Cross(m_forward, m_up);

	m_position += (-x_distance * m_right);
	m_position += (y_distance * m_up);
	m_position += (z_distance * m_forward);

	m_x = XMVectorGetX(m_position);
	m_y = XMVectorGetY(m_position);
	m_z = XMVectorGetZ(m_position);
	
}

void Camera::MoveWithPlayer(float x, float y, float z)
{
	m_position = XMVectorSet(x, y, z, 0.0f);

	m_x = XMVectorGetX(m_position);
	m_y = XMVectorGetY(m_position);
	m_z = XMVectorGetZ(m_position);

}

float Camera::GetNearClipPlane()
{
	return near_clip_plane;
}

XMMATRIX Camera::GetViewMatrix()
{
	m_lookAt = XMVectorSet(m_x + m_dx, m_y + m_dy, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	return XMMatrixLookAtLH(m_position, m_lookAt, m_up);
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projection;
}

XMVECTOR Camera::GetPosition()
{
	return m_position;
}

XMFLOAT3 Camera::GetPos()
{
	XMFLOAT3 pos;

	pos.x = XMVectorGetX(m_position);
	pos.y = XMVectorGetY(m_position);
	pos.z = XMVectorGetZ(m_position);

	return pos;

}