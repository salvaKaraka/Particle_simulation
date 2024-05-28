//external includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib> // rand
#include <ctime>   // rand seed
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//internal includes
#include "physics/Particle.h"
#include "physics/CollisionHandler.h"
#include "renderer/ShaderProgram.h"
#include "renderer/VAO.h"
#include "renderer/EBO.h"
#include "renderer/VBO.h"
#include "renderer/Renderer.h"
#include "interface/Interface.h"

//settings
#include "Settings.h"

void processInput(GLFWwindow* window, CollisionHandler& handler, std::array<float, 2> cursor_pos);
static Particle createRandomParticle();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//globals 
bool left_clicked = false;
bool right_clicked = false;

int g_screenWidth = SCR_WIDTH;
int g_screenHeight = SCR_HEIGHT;


int main() {

    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(g_screenWidth, g_screenHeight, "Particle Simulation", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //RESIZE
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

{

    //Paricles
    srand(time(nullptr));
  
    //Collision Handler
    CollisionHandler handler;

	//square canvas
    float vertices_canvas[] = {
    -1.0f, -1.0f,
    1.0f, -1.0f,
    1.0f, 1.0f,
    -1.0f, 1.0f,
    };
    GLuint indices_canvas[] = {
    0, 1, 2,
    2, 3, 0
    };

    VAO canvasVAO;
    VBO canvasVBO(vertices_canvas, sizeof(vertices_canvas));
    canvasVAO.Bind();
    canvasVAO.LinkAttribute(canvasVBO, 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
	EBO canvasEBO = EBO(indices_canvas, sizeof(indices_canvas) / sizeof(GLuint));       //Order of function calls is EXTREMELY important (bind vao, link attrib, create ebo)! otherwise it will not work (error: (nvoglv64.dll) 0xC0000005 access violation)

    //cursor
    ShaderProgram cursorShaders("src/renderer/shaders/canvasVert.glsl", "src/renderer/shaders/cursorFrag.glsl");

    //particle
    ShaderProgram particleShaders("src/renderer/shaders/canvasVert.glsl", "src/renderer/shaders/particleFrag.glsl");

	//renderer settings
    enableBlending(true);

    //interface
	Interface interface(window);

	//performance
    float frame_start = glfwGetTime();
	float delta_time = 1.f;
	float fps = 1.f / delta_time;

    //containers
    float container_radius = 1.0f;
    float container_width = 1.0f;
    float container_height = 1.0f;
    char* container_type = (char*)"box";

    //RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {

        //Time
        float frame_end = glfwGetTime();
        delta_time = frame_end - frame_start;
        frame_start = frame_end;
        //Performance
        int ms = delta_time * 1000;
        fps = 1.f / delta_time;

        //Update
        handler.updatePositions(delta_time, SUB_STEPS);

        //Imput
        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
        std::array<GLfloat, 2> cursor_pos{ (GLfloat)cursor_x ,  (GLfloat)cursor_y };
        processInput(window, handler, cursor_pos);

        //Rendering
        clear(BG_R, BG_G, BG_B, 1.f);

        //particles
        for (Particle& p : handler.particles) {
			drawParticle(canvasVAO, canvasEBO, particleShaders, p.pos, p.vel, RADIUS, g_screenWidth, g_screenHeight);
        }

		//cursor
        drawCursor(canvasVAO, canvasEBO, cursorShaders, cursor_pos, CURSOR_RADIUS, left_clicked, right_clicked, frame_start, g_screenWidth, g_screenHeight);

		//Interface
		interface.BeginFrame();
        {


            ImGui::Begin("Performance:");
            ImGui::Text("Frame time: %d ms", ms);
			ImGui::Text("FPS: %.0f", fps);
            ImGui::End();

			ImGui::Begin("Container:");
            //size
			ImGui::SliderFloat("Container radius", &container_radius, 0.1f, 1.0f);
			ImGui::SliderFloat("Container width", &container_width, 0.1f, 2.0f);
			ImGui::SliderFloat("Container height", &container_height, 0.1f, 2.0f);
			//type
            if (ImGui::Button("Box")) {
                container_type = (char*)"box";
            }
			ImGui::SameLine();
			if (ImGui::Button("Circle")) {
				container_type = (char*)"circle";
            }
            handler.setContainer(container_type, container_width, container_height, container_radius);



            ImGui::Begin("Controls:");
			ImGui::Text("WASD: Change gravity direction");
			ImGui::Text("R: Restore particles");
			ImGui::Text("N: New particles");
			ImGui::Text("Left click: Attraction");
			ImGui::Text("Right click: Repulsion");
			ImGui::Text("ESC: Exit simulation");
            ImGui::End();
        }
		interface.EndFrame();

        //Check and call events and swap the buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}
	glfwDestroyWindow(window);
    glfwTerminate();
}


void processInput(GLFWwindow* window, CollisionHandler& handler, std::array<float, 2> cursor_pos)
{
    //keyboard
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        handler.changeGravity('u');
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        handler.changeGravity('r');
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        handler.changeGravity('d');
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        handler.changeGravity('l');
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        handler.addParticle(createRandomParticle());
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        handler.removeParticle();
    }

	//mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        left_clicked = true;
        cursor_pos[0] = (cursor_pos[0] - ((float)g_screenWidth / 2.0f)) / ((float)g_screenWidth / 2.0f);
        cursor_pos[1] = (((float)g_screenHeight / 2.0f) - cursor_pos[1]) / ((float)g_screenHeight / 2.0f);
        handler.applyAttraction(cursor_pos, CURSOR_RADIUS, ATTRACTION_STRENGTH);
    }else{
        left_clicked = false;
        handler.applyAttraction(cursor_pos, 0, 0);
    }

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		right_clicked = true;
		cursor_pos[0] = (cursor_pos[0] - ((float)g_screenWidth / 2.0f)) / ((float)g_screenWidth / 2.0f);
		cursor_pos[1] = (((float)g_screenHeight / 2.0f) - cursor_pos[1]) / ((float)g_screenHeight / 2.0f);
		handler.applyAttraction(cursor_pos, CURSOR_RADIUS, REPULSION_STRENGTH);
	}
	else right_clicked = false;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
	g_screenWidth = width;
	g_screenHeight = height;
}

static Particle createRandomParticle() {
    //float xPos = static_cast<float>(std::rand()) / RAND_MAX * 2 - 1;
    //float yPos = static_cast<float>(std::rand()) / RAND_MAX * 2 - 1;
    // Create particle and add to vector
    Particle particle({ -2, 1 }, RADIUS, { 0.f }, { 4, 1 }, { 0.0f });
    return particle;
}

