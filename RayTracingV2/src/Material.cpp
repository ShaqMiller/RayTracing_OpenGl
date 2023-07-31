#include "Material.h"
#include "glm/glm.hpp"

Material::Material() {
	m_albedo = glm::vec3(0);
	m_roughness = 0;
}

Material :: Material(const glm::vec3 &color,const float &roughness) {
	//Init values
	m_albedo = color;
	m_roughness = roughness;
}

Material :: ~Material(){}

glm::vec3 Material::getColor() {
	return m_albedo;
}

float Material::getRoughness() {
	return m_roughness;
}

void Material::setColor(const glm::vec3& color) {
	m_albedo = color;
}

void Material::setRoughness(const float &roughness) {
	m_roughness = roughness;
}


