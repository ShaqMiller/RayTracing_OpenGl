#include "Sphere.h"
#include "glm/glm.hpp"
#include "Material.h"

Sphere::Sphere() {
	m_material = Material();
	m_position = glm::vec3(0);
	m_radius = 0.5f;
}

Sphere::Sphere(Material material, glm::vec3 position,float radius) {
	m_material = material;
	m_position = position;
	m_radius = radius;
};

Sphere::Sphere(Material material) {
	m_material = material;
	m_position = glm::vec3(0);
	m_radius = 0.5f;
};

Sphere::Sphere(glm::vec3 position) {
	m_material = Material();
	m_position = glm::vec3(position);
	m_radius = 0.5f;
};

float Sphere::getRadius() {
	return m_radius;
}
Material Sphere::getMaterial() {
	return m_material;
}
glm::vec3 Sphere::getPosition() {
	return m_position;
}

void Sphere::setMaterial(Material material) {
	m_material = material;
}
void Sphere::setPosition(glm::vec3 pos) {
	m_position = glm::vec3(pos);
}

void Sphere::setRadius(float radius) {
	m_radius = radius;
}



Sphere::~Sphere() {

}

