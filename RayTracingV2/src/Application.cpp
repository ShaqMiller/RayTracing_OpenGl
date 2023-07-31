#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include "glm/glm.hpp"
#include <math.h>
#include "Renderer.h"


struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;

};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        None =-1, VERTEX =0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;

    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                //mode to vertex
                type = ShaderType::VERTEX;
            } 
            else if (line.find("fragment") != std::string::npos) {
                //mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else {
            //Concat Text
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(),ss[1].str() };

}

static unsigned int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //TODO: ERROR HANDLING
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to comiple " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}




int main(void)
{
    GLFWwindow* window;
    const int GLWindowWidth = 800, GLWindowHeight = 620;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(GLWindowWidth, GLWindowHeight, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
        std::cout << "Error Init";

    float positions[] = {
        -1.0f, -1.0f, 0.0f, 0.0f, //0 
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f//3
    };

    unsigned int indices[] = {
       0,1,2,
       2,3,0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //Create buffer
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    //Select buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    //Specify the data
    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), positions, GL_STATIC_DRAW);

    //Enable the attrib
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //Define Attrib
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);         //pos
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); //tex

    //INDECIES
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    //Select buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    //Specify the data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    Renderer renderer = Renderer(GLWindowWidth, GLWindowHeight);
  

    ///Shader
    ShaderProgramSource shaderSrc = ParseShader("res/shaders/Basic.shader");
    unsigned int shader = CreateShader(shaderSrc.VertexSource, shaderSrc.FragmentSource);
    glUseProgram(shader);

    //Unifroms -> after bound shader
    int colorLocation = glGetUniformLocation(shader, "u_Color");
    int textureLocation = glGetUniformLocation(shader, "u_Texture");

    glUniform4f(colorLocation, 0.2f, .03f, 0.8f, 1.0f);
    glUniform1i(textureLocation, 0);
    
    renderer.Render();
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, nullptr);

       
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    //glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
