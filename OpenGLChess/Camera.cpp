#include "Camera.h"


void Camera::Translate(float x, float y, float z)
{
	m_position += glm::vec3(x, y, z);
}
void Camera::Translate(glm::vec3 offset)
{
	m_position += offset;
}

void Camera::Rotate(float x, float y, float z)
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;

	// Clamping the pitch value to avoid gimbal lock
	if (m_rotation.x > glm::radians(89.0f))
		m_rotation.x = glm::radians(89.0f);
	if (m_rotation.x < glm::radians(-89.0f))
		m_rotation.x = glm::radians(-89.0f);
	m_rotation += glm::vec3(x, y, z);

}

void Camera::UpdateVPMat(int frameWidth,int frameHieght)
{
	

	m_view = glm::mat4(1.0f);
	m_projection = glm::mat4(1.0f);

	// Apply rotations
	m_view = glm::rotate(m_view, m_rotation.x, glm::vec3(1, 0, 0));
	m_view = glm::rotate(m_view, m_rotation.y, glm::vec3(0, 1, 0));
	m_view = glm::rotate(m_view, m_rotation.z, glm::vec3(0, 0, 1));

	// Apply translation (notice the negative sign for the camera position)
	m_view = glm::translate(m_view, -m_position);

	// Create the rotation matrix separately
	m_rotationMat = glm::mat4(1.0f);
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.x, glm::vec3(1, 0, 0));
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.y, glm::vec3(0, 1, 0));
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.z, glm::vec3(0, 0, 1));

	// Set up the projection matrix (Perspective)
	m_projection = glm::perspective(glm::radians(m_FOV), (float)frameWidth / (float)frameHieght, 0.1f, 100.0f);
	forward =glm::vec3( m_view[0][2], m_view[1][2], m_view[2][2]);
	right = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);


}
