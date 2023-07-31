#pragma once
#include "Sphere.h"
#include <vector>
#include "Light.h"

struct Scene
{
	std::vector<Sphere> m_spheres;
	std::vector<Light> m_lights;

public:
	Scene();
	~Scene();
	void addSpehere();
	void addLight();
	void addLight(Light light);

	void addSpehere(const Material& mat);
	void addSpehere(const glm::vec3& pos);
	void addSpehere(const Material& mat, const glm::vec3& pos,const float& radius);
	void addSpehere(const Sphere& sphere);

};

