#include "Camera.h"


void Camera::Translate(float x, float y, float z)
{
	m_position += glm::vec3(x, y, z);
}
void Camera::Translate(glm::vec3 offset)
{
	m_position += glm::vec3(offset.x, offset.y, offset.z);
}

void Camera::Rotate(float x, float y, float z)
{
	m_rotation += glm::vec3(x, y, z);

}

void Camera::UpdateVPMat(int frameWidth,int frameHieght)
{
	

	m_view = glm::mat4(1.0f);
	m_projection = glm::mat4(1.0f);
	m_view = glm::rotate(m_view, m_rotation.x, glm::vec3(1, 0, 0));
	m_view = glm::rotate(m_view, m_rotation.y, glm::vec3(0, 1, 0));
	m_view = glm::rotate(m_view, m_rotation.z, glm::vec3(0, 0, 1));

	m_view = glm::translate(m_view, m_position);

	m_rotationMat = glm::mat4(1.0f);
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.x, glm::vec3(1, 0, 0));
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.y, glm::vec3(0, 1, 0));
	m_rotationMat = glm::rotate(m_rotationMat, m_rotation.z, glm::vec3(0, 0, 1));
	m_projection = glm::perspective(glm::radians(m_FOV), (float)frameWidth / (float)frameHieght, 0.1f, 100.0f);

	forward = m_rotationMat * glm::vec4(0, 0, 1, 0);
	forward = glm::normalize(forward);
	forward = glm::vec3(-forward.x, -forward.y, forward.z);
	right = m_rotationMat * glm::vec4(1, 0, 0,0);
	forward = glm::normalize(right);


}
