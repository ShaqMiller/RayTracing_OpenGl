#pragma once
#include "Scene.h"
#include "Ray.h"

class Renderer
{
private:
	Scene m_scene;
	int m_glWidth;
	int m_glHeight;

public:
	Renderer(int glWidth,int glHeight);
	~Renderer();

	unsigned int Render();
	glm::vec3 TraceRay(const Ray& ray, const Scene& scene);
	void BobVertically(Scene scene);

};


