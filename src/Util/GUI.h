#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

struct GLFWwindow;

class GUI {
public:
    GUI(GLFWwindow* window);
    ~GUI();

    void NewFrame();
    void Render();

    void DrawPerformancePanel(int objectCount);

private:
    void SetupStyle();

    std::vector<float> FpsHistory;
    const int MaxHistorySize = 90;
};