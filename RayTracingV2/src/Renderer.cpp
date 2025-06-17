#include "Renderer.h"
#include "Scene.h"
#include "Ray.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Light.h"

Renderer::Renderer(int glWidth, int glHeight) {
    m_glWidth = glWidth;
    m_glHeight = glHeight;

    {
        Sphere sphere1 = Sphere();
        sphere1.m_material = Material(glm::vec3(1.0f, 0.0f, 0.8f), 0.5f);
        sphere1.m_position = glm::vec3(0.0f, 0.0f, -12.0f);
        sphere1.m_radius = 1.9f;
        m_scene.addSpehere(sphere1);

        Sphere sphere2 = Sphere();
        sphere2.m_material = Material(glm::vec3(0.5f, 1.0f, 0.8f), 0.5f);
        sphere2.m_position = glm::vec3(10.0f, 10.0f, -30.0f);
        sphere2.m_radius = 2.9f;
        m_scene.addSpehere(sphere2);
    }

    {
        Light light = Light();
        light.intensity = 0.8f;
        light.color = glm::vec3(0.3f, 0.3f, 0.0f);
        m_scene.addLight(light);
    }
  
    
}
Renderer::~Renderer() {
}

Scene* Renderer::getScene(){
    return &m_scene;
}

unsigned int Renderer::Render(){
    const int textureWidth = 800;
    const int textureHeight = 620;

    GLubyte *textureData = new GLubyte[textureWidth * textureHeight * 3];
    //GLubyte textureData [textureWidth * textureHeight * 3];

    int curInd = 0;

	Ray ray = Ray();
	ray.Origin = glm::vec3(0.0f, 0.0f, 0.0f);

    float aspectRatio = (float)textureWidth / (float)textureHeight;

    for (int y = 0; y < textureHeight; ++y) {
        for (int x = 0; x < textureWidth; ++x) {
            //Coord based on 0->1
            glm::vec2 coord = { (float)x / (float)textureWidth,(float)y / (float)textureHeight };
            coord = coord * 2.0f - 1.0f; //from 0-1 to -1 -> 1
            coord.x *= aspectRatio; // Correct horizontal scaling


            ray.Direction = glm::vec3(coord, -1.0f);
            ray.Direction = glm::normalize(ray.Direction);


            glm::vec3 pixelColor = TraceRay(ray, m_scene);

            textureData[curInd + 0] = static_cast<GLubyte>(pixelColor.x * 255); // Red
            textureData[curInd + 1] = static_cast<GLubyte>(pixelColor.y * 255); // Green
            textureData[curInd + 2] = static_cast<GLubyte>(pixelColor.z * 255); // Blue;                          
            curInd+=3;
        }
    }

    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Texture settings
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    delete[] textureData;

    return textureID;
}

glm::vec3 Renderer::TraceRay(const Ray& ray, const Scene& scene) {

    //Run a ray
    // (bx^2 + by^2 + bz^2)t^2 + (2(axbx + ayby + azbz))t + (ax^2 + ay^2 + az^2 -r^2) = 0

    //a = ray origin
    //b = ray direction
    //r = radius of sphere
    //t = hit distance

    if (scene.m_spheres.size() == 0) {
        return glm::vec3(0.0f,0.0f,0.5f);
    }

    const Sphere* closestSphere = nullptr;
    float hitDistance = FLT_MAX;

    for (const Sphere& sphere:scene.m_spheres) {

        glm::vec3 origin = ray.Origin - sphere.m_position;

        float a = glm::dot(ray.Direction, ray.Direction);
        float b = 2.0f * glm::dot(origin, ray.Direction);
        float c = glm::dot(origin, origin) - sphere.m_radius * sphere.m_radius;


        //Quadratic formula descriminant
        //b^2 -4ac

        float discriminant = b * b - 4.0f * a * c;
        if (discriminant < 0.0f) {
            continue;
        }


        //Where hit?
        //(-b+- sqrt(discriminant))/(2.0f * a)
        float closestT = (-b - glm::sqrt(discriminant)) / (2.0f * a);
        // float t0 = (-b + glm::sqrt(discriminant)) / (2.0f * a); //Second hit

        if (closestT > 0.0f && closestT < hitDistance) {
            hitDistance = closestT;
            closestSphere = &sphere;
        }

    }
    
    //No hits
    if (closestSphere == nullptr) {
        return glm::vec3(0.0f, 0.0f, 0.0f);
    }


    glm::vec3 origin = ray.Origin - closestSphere->m_position;
    glm::vec3 hitPoint = origin + ray.Direction * hitDistance;


    //Normal
    //glm::vec3 normal = hitPoint - sphereOrigin;
    //normal = glm::normalize(normal);

    glm::vec3 normal = glm::normalize((hitPoint));
    //normal = glm::clamp(normal, glm::vec3(0.0f), glm::vec3(1.0f)); //cos(angle)
    //normal = (normal + 1.0f) / 2.0f;

    glm::vec3 color = closestSphere->m_material.m_albedo;

    //Loop through lights
    for (const Light& light : scene.m_lights) {
        glm::vec3 lightDir = glm::normalize(light.direction);
        //angle between light
        float lightInfluence = glm::max(glm::dot(normal, -lightDir), 0.0f); //cos(angle)

        color *= (lightInfluence* light.intensity);
    }
    
    return color;

}


void  Renderer::BobVertically(Scene scene) {
    //float centerPivot = 0.0f;
    float maxDistance = 50.0f;

    //loop thrpguh elements
    int sizeOfSpheres = m_scene.m_spheres.size();

    for (int i = 0;i < sizeOfSpheres;i++) {
        Sphere* sphere = &m_scene.m_spheres[i];
        float verticalVelPower = 1.1f;

        //Give it an intial velocity if set to 0
        if (sphere->m_velocity == glm::vec3(0)) {
            sphere->m_velocity.y = verticalVelPower;
        }


        //Check if out of bounds
        if (sphere->m_position.y > maxDistance) {
            //change vel to go down
            sphere->m_velocity.y = -verticalVelPower;

        }
        else if (sphere->m_position.y < -maxDistance) {
            //change vel to go up
            sphere->m_velocity.y = verticalVelPower;

        }
        //Add vel to position
        sphere->m_position.y += sphere->m_velocity.y;
    }

}
