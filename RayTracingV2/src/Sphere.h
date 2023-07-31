#pragma once
#include "glm/glm.hpp"
#include "Material.h"

struct Sphere
{
public:
	Material m_material;
	glm::vec3 m_position;
	float m_radius;
	glm::vec3 m_velocity = glm::vec3(0);

public:
	Sphere();
	Sphere(Material material, glm::vec3 position, float m_radius);
	Sphere(Material material);
	Sphere(glm::vec3 position);
	

	~Sphere();

	Material getMaterial();
	glm::vec3 getPosition();
	float getRadius();

	void setMaterial(Material material);
	void setPosition(glm::vec3 pos);
	void setRadius(float radius);

};

