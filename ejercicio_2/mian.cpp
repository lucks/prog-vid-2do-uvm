
#include <iostream>
#include <vector>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Vertex Shader
const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uTransform;

void main()
{
    gl_Position = uTransform * vec4(aPos, 1.0);
}
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
#version 330 core

uniform vec3 uColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(uColor, 1.0);
}
)";

// Compilar Shader
unsigned int compileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "Error Shader: " << infoLog << std::endl;
    }

    return shader;
}

// Crear programa Shader
unsigned int createShaderProgram()
{
    unsigned int vertexShader =
        compileShader(GL_VERTEX_SHADER, vertexShaderSource);

    unsigned int fragmentShader =
        compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main()
{
    // Inicializar GLFW
    if (!glfwInit())
    {
        std::cout << "Error al iniciar GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(
        800, 600, "Segundo Parcial - Gema", nullptr, nullptr
    );

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Inicializar GLAD v2
    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "Error al iniciar GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    // Crear geometría de la gema
    // 6 triángulos = 18 vértices
    std::vector<glm::vec3> vertices = {

        // Triángulo 1
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 0.0f,  0.6f, 0.0f),
        glm::vec3( 0.5f,  0.3f, 0.0f),

        // Triángulo 2
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 0.5f,  0.3f, 0.0f),
        glm::vec3( 0.5f, -0.3f, 0.0f),

        // Triángulo 3
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 0.5f, -0.3f, 0.0f),
        glm::vec3( 0.0f, -0.6f, 0.0f),

        // Triángulo 4
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3( 0.0f, -0.6f, 0.0f),
        glm::vec3(-0.5f, -0.3f, 0.0f),

        // Triángulo 5
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3(-0.5f, -0.3f, 0.0f),
        glm::vec3(-0.5f,  0.3f, 0.0f),

        // Triángulo 6
        glm::vec3( 0.0f,  0.0f, 0.0f),
        glm::vec3(-0.5f,  0.3f, 0.0f),
        glm::vec3( 0.0f,  0.6f, 0.0f)
    };

    // Crear VAO y VBO
    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(glm::vec3),
        vertices.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE,
        sizeof(glm::vec3), (void*)0
    );

    glEnableVertexAttribArray(0);

    // Crear Shader Program
    unsigned int shaderProgram = createShaderProgram();

    // Obtener ubicaciones de los uniform
    int transformLoc =
        glGetUniformLocation(shaderProgram, "uTransform");

    int colorLoc =
        glGetUniformLocation(shaderProgram, "uColor");

    // Color morado
    glm::vec3 color(0.55f, 0.15f, 0.85f);

    // Ciclo principal
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.08f, 0.08f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // =====================================
        // APLICACIÓN DE TRANSFORMACIONES
        // =====================================

        glm::mat4 transform = glm::mat4(1.0f);

        // 1. TRASLACIÓN
        // Desplazar 0.4 unidades hacia la derecha.

        // COMPLETAR:
        // transform = glm::translate(...);


        // 2. ESCALAMIENTO
        // Reducir el tamaño al 70%.

        // COMPLETAR:
        // transform = glm::scale(...);


        // 3. ROTACIÓN
        // Girar 45 grados sobre el eje Z.

        // COMPLETAR:
        // transform = glm::rotate(...);


        // Enviar matriz al Vertex Shader
        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
        );

        // Enviar color
        glUniform3fv(
            colorLoc,
            1,
            glm::value_ptr(color)
        );

        // Dibujar gema
        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES,
            0,
            static_cast<GLsizei>(vertices.size())
        );

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberar recursos
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();

    return 0;
}
