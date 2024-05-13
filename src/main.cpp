#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Particle.h"
#include <CollisionHandler.h>
#include <cstdlib> // rand
#include <ctime>   // rand seed


//Settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

void processInput(GLFWwindow* window, CollisionHandler& handler, std::vector<Particle>& particlesRestore);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

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
    const int numParticles = 30;

    // Ranges
    const float positionRange = 2.0f; 
    const float velocityRange = 5.0f; 
    const float massRange = 5.0f;     

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
        float red = static_cast<float>(rand()) / RAND_MAX;
        float green = static_cast<float>(rand()) / RAND_MAX;
        float blue = static_cast<float>(rand()) / RAND_MAX;

        // Creamos la partícula y la agregamos al vector
        Particle particle({ xPos, yPos }, mass, { red, green, blue }, { xVel, yVel }, { 0.0f, 0.0f });
        particles.push_back(particle);
    }
    std::vector<Particle> particlesRestore = particles;

    CollisionHandler handler(particles, 2, 2, .0033333f, 9.8);

    //RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        //Imput
        processInput(window, handler, particlesRestore);

        //Rendering
        glClearColor(0.f, 0.f, 0.f, 0);  //BG color
        glClear(GL_COLOR_BUFFER_BIT);
        handler.updatePositions();

        //Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window, CollisionHandler& handler, std::vector<Particle>& particlesRestore)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        handler.setParticles(particlesRestore);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}