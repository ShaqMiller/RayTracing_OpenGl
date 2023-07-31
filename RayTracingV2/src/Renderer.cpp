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
        Sphere sphere2 = Sphere();
        sphere2.m_material = Material(glm::vec3(1.0f, 0.0f, 0.8f), 0.5f);
        sphere2.m_position = glm::vec3(1.0f, 0.0f, -2.0f);
        sphere2.m_radius = 2.0f;
        m_scene.addSpehere(sphere2);
    }

    {
        Light light = Light();
        light.intensity = 0.8f;
        m_scene.addLight(light);
    }
  
    
}
Renderer::~Renderer() {
}

void Renderer::Render(){
    const int textureWidth = 700;
    const int textureHeight = 700;

    GLubyte *textureData = new GLubyte[textureWidth * textureHeight * 3];
    //GLubyte textureData [textureWidth * textureHeight * 3];

    int curInd = 0;

	Ray ray = Ray();
	ray.Origin = glm::vec3(0.0f, 0.0f, 3.0f);

    float aspectRatio = (float)m_glWidth / (float)m_glHeight;

    for (int y = 0; y < textureHeight; ++y)
    {
        for (int x = 0; x < textureWidth; ++x)
        {
            //CAMERA TEST
            //const glm::mat4 projection = glm::perspectiveFov(glm::radians(90.0f), (float)m_glWidth, (float)m_glHeight, 0.1f, 200.0f);
            //const glm::mat4 view = glm::lookAt(ray.Origin, ray.Origin + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0, 1, 0));
            //const glm::mat4 inverseView = glm::inverse(view);


            //Coord based on 0->1
            glm::vec2 coord = { (float)x / (float)textureWidth,(float)y / (float)textureHeight };
            coord = coord * 2.0f - 1.0f; //from 0-1 to -1 -> 1

            //CAMERA TEST
            //glm::vec4 target = glm::inverse(projection) * glm::vec4(coord.x, coord.y,1,1);
            //glm::vec3 calculatedRayDir = glm::vec3(inverseView* glm::vec4(glm::normalize(glm::vec3(target) / target.w),0));

            ray.Direction = glm::vec3(coord, -1.0f);
            ray.Direction.x *= aspectRatio;
            ray.Direction = glm::normalize(ray.Direction);



            glm::vec3 pixelColor = TraceRay(ray, m_scene);
            //std::cout << pixelColor.r << "  after *255:" << pixelColor.r * 255 << std::endl;
            textureData[curInd] = static_cast<GLubyte>(pixelColor.r * 255.0f);   // Red component
            textureData[curInd + 1] = static_cast<GLubyte>(pixelColor.g * 255.0f);   // Green component
            textureData[curInd + 2] = static_cast<GLubyte>(pixelColor.b * 255.0f);   // Blue component
            curInd += 3;
        }
    }
    
    //BobVertically(m_scene);

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glEnable(GL_TEXTURE_2D);

    delete[] textureData;
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

        if (closestT < hitDistance) {
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
