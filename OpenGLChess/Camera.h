#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>
class Camera {
public :
	float m_FOV;
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::mat4x4 m_view;
	glm::mat4x4 m_projection;
	glm::mat4x4 m_rotationMat;
	glm::vec3 forward=glm::vec3(0,0,0);
	glm::vec3 right = glm::vec3(0,0,0);
public :
	void Translate(glm::vec3 offset);
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void UpdateVPMat(int frameWidth, int frameHieght);

};