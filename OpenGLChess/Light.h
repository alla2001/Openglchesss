#pragma once
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <vector>

struct  Light
{
	float intensity=1.0f;
	float ambient;
	glm::vec3 color;
	glm::vec3 pos;
	std::vector<glm::mat4> shadowTransforms;
	float shadowFarPlane;
	float m_shadowWidth = 1024;
	float m_shadowHeight = 1024;
	unsigned int m_depthCubemap=121;
	glm::mat4 m_shadowProj;
	void ConstructShadowMatrix();
	void InitShadows();
	void InitRenderShadows(  unsigned int frameBuffer);
};