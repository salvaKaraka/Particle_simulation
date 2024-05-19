#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Particle.h"
#include <CollisionHandler.h>
#include <cstdlib> // rand
#include <ctime>   // rand seed
#include <ShaderProgram.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

const float RADIUS = 0.004f;
const float PI = 3.14159265359f;
const unsigned int STEPS = 20;
const float ANGLE = PI * 2.f / STEPS;

void processInput(GLFWwindow* window, CollisionHandler& handler, std::vector<Particle>& particlesRestore);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();

    //INITIALIZATION
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PhysicsEngine", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //RESIZE
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //Paricles
    srand(time(nullptr));
    std::vector<Particle> particles;
    const int numParticles = 3500;

    // Ranges
    const float positionRange = 2.0f;
    const float velocityRange = 5.0f;
    const float massRange = 5.0f;
    const float colorRange = 1.f;

    for (int i = 0; i < numParticles; ++i) {
        //Position
        float xPos = static_cast<float>(rand()) / RAND_MAX * positionRange - positionRange / 2;
        float yPos = static_cast<float>(rand()) / RAND_MAX * positionRange - positionRange / 2;

        //Veelocity
        float xVel = static_cast<float>(rand()) / RAND_MAX * velocityRange - velocityRange / 2;
        float yVel = static_cast<float>(rand()) / RAND_MAX * velocityRange - velocityRange / 2;

        //Mass
        //float mass = static_cast<float>(rand()) / RAND_MAX * massRange + 1.f; // Aseguramos que la masa sea mayor que 1
        float mass = 4.f;
        //color
        float red = 0.01f + (static_cast<float>(rand()) / RAND_MAX);
        float green = 0.01f + (static_cast<float>(rand()) / RAND_MAX);
        float blue = 0.01f + (static_cast<float>(rand()) / RAND_MAX);

        // Creamos la partícula y la agregamos al vector
        Particle particle({ xPos, yPos }, RADIUS, { red, green, blue }, { xVel, yVel }, { 0.0f, 0.0f });
        particles.push_back(particle);
    }
    std::vector<Particle> particlesRestore = particles;

    CollisionHandler handler(particles, 2, 2, .0034f, 9.8);

    ShaderProgram shaderProgram("Shaders/vertex.glsl", "Shaders/fragment.glsl");

    //cargo la informacion de los vertices en la GPU
    GLuint buffer_id;
    {
        //vertices de un circulo
        std::vector<float> vertices;
        for (int i = 0; i < STEPS; ++i) {
            float x = RADIUS * cos(ANGLE * i);
            float y = RADIUS * sin(ANGLE * i);
            vertices.push_back(x);
            vertices.push_back(y);
        }
        //buffer de vertices

        glGenBuffers(1, &buffer_id);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_id);															//selecciono el buffer con su id
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);	//cargo los datos (vertices en este caso) en el buffer que seleccione previamente
        //indico a opengl los atributos guardados en el buffer (Vertex Attributes)
        glEnableVertexAttribArray(0);																		//activo el atributo 0
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);						//indico que cada vertice tiene 2 floats de posicion
        glBindBuffer(GL_ARRAY_BUFFER, 0); 																	//deselecciono el buffer para evitar modificaciones accidentales
    }


    //RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        //Imput
        processInput(window, handler, particlesRestore);

        //Rendering
        glClearColor(0.f, 0.f, 0.f, 0);  //BG color
        glClear(GL_COLOR_BUFFER_BIT);

        handler.updatePositions();

        //ciclo de renderizado de las particulas
        shaderProgram.Use();																					//activo el programa de shaders

        //uniforms
        unsigned int transformLoc = glGetUniformLocation(shaderProgram.GetID(), "transform");				//obtengo la ubicacion de la variable uniforme "transform"
        GLint color_location = glGetUniformLocation(shaderProgram.GetID(), "color");							//obtengo la ubicacion de la variable uniforme "color"

        for (Particle& p : handler.getParticles()) {
            glUniform4fv(color_location, 1, p.color.data());														//le paso el color al shader

            glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(p.pos[0], p.pos[1], 0.0f));		//aplico transformacion de traslacion
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix)); 						//le paso la matriz de transformacion al shader
            glDrawArrays(GL_TRIANGLE_FAN, 0, STEPS);																//dibujo el circulo
        }

        //Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}


void processInput(GLFWwindow* window, CollisionHandler& handler, std::vector<Particle>& particlesRestore)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        handler.setParticles(particlesRestore);
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        std::vector<Particle> particles;
        const int numParticles = 3500;

        // Ranges
        const float positionRange = 2.0f;
        const float velocityRange = 5.0f;
        const float massRange = 5.0f;
        const float colorRange = 1.f;

        for (int i = 0; i < numParticles; ++i) {
            //Position
            float xPos = static_cast<float>(rand()) / RAND_MAX * positionRange - positionRange / 2;
            float yPos = static_cast<float>(rand()) / RAND_MAX * positionRange - positionRange / 2;

            //Veelocity
            float xVel = static_cast<float>(rand()) / RAND_MAX * velocityRange - velocityRange / 2;
            float yVel = static_cast<float>(rand()) / RAND_MAX * velocityRange - velocityRange / 2;

            //Mass
            //float mass = static_cast<float>(rand()) / RAND_MAX * massRange + 1.f; // Aseguramos que la masa sea mayor que 1
            float mass = 4.f;
            //color
            float red = 0.01f + (static_cast<float>(rand()) / RAND_MAX);
            float green = 0.01f + (static_cast<float>(rand()) / RAND_MAX);
            float blue = 0.01f + (static_cast<float>(rand()) / RAND_MAX);

            // Creamos la partícula y la agregamos al vector
            Particle particle({ xPos, yPos }, RADIUS, { red, green, blue }, { xVel, yVel }, { 0.0f, 0.0f });
            particles.push_back(particle);
        }
        handler.setParticles(particles);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}