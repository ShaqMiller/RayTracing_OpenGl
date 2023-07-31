#include "Scene.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "Light.h"

Scene::Scene() {
}
Scene::~Scene() {
}


void Scene::addSpehere() {
	Material mat1 = Material(glm::vec3(1, 0.5f, 0), 0.5f);
	m_spheres.push_back(Sphere(mat1, glm::vec3(0),0.5f));
}
void Scene::addLight() {
	m_lights.push_back(Light());
}
void Scene::addLight(Light light) {
	m_lights.push_back(light);
}

void Scene::addSpehere(const Material& mat) {
	m_spheres.push_back(Sphere(mat, glm::vec3(0),0.5f) );
}
void Scene::addSpehere(const glm::vec3& pos) {
	Material mat1 = Material(glm::vec3(1, 0.5f, 0), 0.5f);
	m_spheres.push_back(Sphere(mat1, pos,0.5f));
}

void Scene::addSpehere(const Material& mat,const glm::vec3& pos, const float& radius) {
	m_spheres.push_back(Sphere(mat, pos, radius));
}

void Scene::addSpehere(const Sphere& sphere) {
	m_spheres.push_back(sphere);
}


