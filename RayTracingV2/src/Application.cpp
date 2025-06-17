#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Renderer.h"

const int GLWindowWidth = 800;
const int GLWindowHeight = 620;

const int textureWidth = 256;
const int textureHeight = 256;

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

    Renderer renderer = Renderer(GLWindowWidth, GLWindowHeight);
    unsigned int textureID = renderer.Render();

    

    GLuint shader = CreateShaderProgram();
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "u_Texture"), 0); // Bind texture unit 0

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);

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
