#pragma once
#include "glm/vec3.hpp"

struct  Light
{
	float intensity=1.0f;
	float ambient;
	glm::vec3 color;
	glm::vec3 pos;
};