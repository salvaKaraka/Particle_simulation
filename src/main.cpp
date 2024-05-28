//external includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib> // rand
#include <ctime>   // rand seed
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utils/SKMath.h>

//internal includes
#include "physics/VerletObject.h"
#include "physics/PhysicsEngine.h"

#include "renderer/ShaderProgram.h"
#include "renderer/VAO.h"
#include "renderer/EBO.h"
#include "renderer/VBO.h"
#include "renderer/Renderer.h"
#include "interface/Interface.h"

//settings
#include "Settings.h"

void processInput(GLFWwindow* window, PhysicsEngine& physics, Vec2 cursor_pos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//globals 
bool left_clicked = false;
bool right_clicked = false;

int g_screenWidth = SCR_WIDTH;
int g_screenHeight = SCR_HEIGHT;

float g_radius = 0.05f;

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
	PhysicsEngine physics;

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

	physics.setFrameRate(60.0f);
	physics.setSubSteps(SUB_STEPS);

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
        physics.udateSubSteps();

        //Imput
        double cursor_x, cursor_y;
        glfwGetCursorPos(window, &cursor_x, &cursor_y);
        Vec2 cursor_pos( cursor_x , cursor_y );
        processInput(window, physics, cursor_pos);

        //Rendering
        clear(BG_R, BG_G, BG_B, 1.f);

        //particles
        for (VerletObject& object : physics.getVerletObjects()) {
			float abs_vel = object.getVelocity(physics.getStepDt()).magnitude();
			drawParticle(canvasVAO, canvasEBO, particleShaders, object.current_pos, abs_vel , object.radius, g_screenWidth, g_screenHeight);
        }

		//cursor
        drawCursor(canvasVAO, canvasEBO, cursorShaders, cursor_pos, CURSOR_RADIUS, left_clicked, right_clicked, frame_start, g_screenWidth, g_screenHeight);

		//Interface
		interface.BeginFrame();
        {


            ImGui::Begin("Performance:");
            ImGui::Text("Frame time: %d ms", ms);
			ImGui::Text("FPS: %.0f", fps);
            ImGui::Text("Ammount of objects: %d", physics.getVerletObjects().size());
            ImGui::End();


			ImGui::Begin("Settings:");
            //size
            ImGui::SliderFloat("Particle radius", &g_radius, 0.01f, 0.1f);

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
            physics.setContainer(container_type, container_width, container_height, container_radius);



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


void processInput(GLFWwindow* window, PhysicsEngine& physics, Vec2 cursor_pos)
{
    //keyboard
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        physics.setGravity(Vec2(0.0f,-GRAVITY));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        physics.setGravity(Vec2(-GRAVITY,0.0f));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        physics.setGravity(Vec2(0.0f, GRAVITY));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        physics.setGravity(Vec2(GRAVITY, 0.0f));
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		float x = (rand() % 100) / 100.0f - 0.5f;
		float y = (rand() % 100) / 100.0f - 0.5f;
        physics.createVerletObject(Vec2(x,y),g_radius);
    }
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        physics.removeVerletObject();
    }

	//mouse
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        left_clicked = true;
        cursor_pos.x = (cursor_pos.x - ((float)g_screenWidth / 2.0f)) / ((float)g_screenWidth / 2.0f);
        cursor_pos.y = (((float)g_screenHeight / 2.0f) - cursor_pos.y) / ((float)g_screenHeight / 2.0f);
        physics.applyAttraction(cursor_pos, CURSOR_RADIUS, ATTRACTION_STRENGTH);
    }else{
        left_clicked = false;
        physics.applyAttraction(cursor_pos, 0, 0);
    }

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
		right_clicked = true;
        cursor_pos.x = (cursor_pos.x - ((float)g_screenWidth / 2.0f)) / ((float)g_screenWidth / 2.0f);
        cursor_pos.y = (((float)g_screenHeight / 2.0f) - cursor_pos.y) / ((float)g_screenHeight / 2.0f);
        physics.applyAttraction(cursor_pos, CURSOR_RADIUS, REPULSION_STRENGTH);
	}
	else right_clicked = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
	g_screenWidth = width;
	g_screenHeight = height;
}

