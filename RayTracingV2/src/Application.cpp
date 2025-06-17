#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"

const int GLWindowWidth = 800;
const int GLWindowHeight = 620;

const int textureWidth = 256;
const int textureHeight = 256;
Renderer* g_renderer = nullptr;

// Vertex + Fragment Shader source
const char* vertexShaderSrc = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 fragTexCoord;

void main() {
    fragTexCoord = aTexCoord;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSrc = R"(
#version 330 core
out vec4 FragColor;

in vec2 fragTexCoord;
uniform sampler2D u_Texture;

void main() {
    FragColor = texture(u_Texture, fragTexCoord);
}
)";

// Compile shader
GLuint CompileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;
    }

    return shader;
}

// Create shader program
GLuint CreateShaderProgram() {
    GLuint vertex = CompileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}


// Mouse callback to update light position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!g_renderer) return;  // Renderer not initialized yet

    Scene* scene = g_renderer->getScene();
    if (!scene) return;
    
    Light* firstLight = scene->getLightByIndex(0);
    if (!firstLight) return;

    // Get window size for normalization
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // Normalize mouse coordinates from [0, width] and [0, height] to [-1, 1]
    float normX = (float(xpos) / width) * 2.0f - 1.0f;
    float normY = 1.0f - (float(ypos) / height) * 2.0f;  // Invert Y axis

    // Update light position X and Y based on mouse position
    // Assuming Light has glm::vec3 position or similar
    firstLight->direction.x = normX ;
    firstLight->direction.y = normY;
    // Optional: keep z as is or set some fixed depth
    // firstLight->position.z = some_value;
}



int main() {
    // Init GLFW
    if (!glfwInit())
        return -1;

    // Create Window
    GLFWwindow* window = glfwCreateWindow(GLWindowWidth, GLWindowHeight, "Gradient Texture", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwSetCursorPosCallback(window, cursor_position_callback); //for controlling light sources

    glfwMakeContextCurrent(window);

    // Init GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW Init Failed!" << std::endl;
        return -1;
    }

    // Quad covering screen
    float quad[] = {
        //  positions     tex coords
        -1.0f, -1.0f,     0.0f, 0.0f,
         1.0f, -1.0f,     1.0f, 0.0f,
         1.0f,  1.0f,     1.0f, 1.0f,
        -1.0f,  1.0f,     0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Buffers
    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Attributes
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    g_renderer = new Renderer(GLWindowWidth, GLWindowHeight);
    unsigned int textureID = g_renderer->Render();

    

    GLuint shader = CreateShaderProgram();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0); // Bind texture unit 0

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        textureID = g_renderer->Render();
        glUseProgram(shader);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteProgram(shader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteTextures(1, &textureID);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
