#pragma once
#include "glm/glm.hpp"

struct Material
{
public:
	glm::vec3 m_albedo;
	float m_roughness;

public:
	Material();
	Material(const glm::vec3& color, const float& roughness);
	~Material();


	//Setters
	void setRoughness(const float &roughness);
	void setColor(const glm::vec3 &color);

	//Getters
	glm::vec3 getColor();
	float getRoughness();

};

