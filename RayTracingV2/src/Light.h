#pragma once
#include "glm/glm.hpp"

struct Light
{
	glm::vec3 position = {-1.0f,-1.0f ,-1.0f };
	glm::vec3 direction = { -1.0f,-1.0f ,-1.0f };
	glm::vec3 color = {1.0f,1.0f,1.0f};
	float intensity = 1.0f;
};

