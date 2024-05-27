#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Interface {
public:
    Interface(GLFWwindow* window);
    ~Interface();

    void BeginFrame();
    void EndFrame();
    void Shutdown(); // Asegúrate de que exista una definición correspondiente en el cpp.
};
