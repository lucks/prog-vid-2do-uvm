
#include <iostream>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 uColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

unsigned int createShader(
    unsigned int type,
    const char* source)
{
    unsigned int shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Error Shader: " << infoLog << std::endl;
    }

    return shader;
}

int main()
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        std::cout << "Error al iniciar GLFW";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        WIDTH, HEIGHT,
        "Examen OpenGL",
        NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLAD v2
    if (!gladLoadGL(
        (GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Error al iniciar GLAD";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // Compilar shaders
    unsigned int vertexShader =
        createShader(GL_VERTEX_SHADER,
                     vertexShaderSource);

    unsigned int fragmentShader =
        createShader(GL_FRAGMENT_SHADER,
                     fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int linkSuccess;
    glGetProgramiv(
        shaderProgram,
        GL_LINK_STATUS,
        &linkSuccess);

    if (!linkSuccess)
    {
        char infoLog[512];
        glGetProgramInfoLog(
            shaderProgram, 512, NULL, infoLog);

        std::cout << "Error al enlazar shaders: "
                  << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // =====================================
    // ZONA QUE MODIFICARÁ EL ESTUDIANTE
    // =====================================

    std::vector<glm::vec3> vertices = {
        glm::vec3(-0.5f, -0.5f, 0.0f),
        glm::vec3( 0.5f, -0.5f, 0.0f),
        glm::vec3( 0.0f,  0.5f, 0.0f)
    };

    // =====================================

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW);

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Color inicial
    float colorR = 0.2f;
    float colorG = 0.7f;
    float colorB = 1.0f;

    // Ciclo principal
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        int colorLocation = glGetUniformLocation(
            shaderProgram, "uColor");

        glUniform3f(
            colorLocation,
            colorR, colorG, colorB);

        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            (GLsizei)vertices.size());

        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
