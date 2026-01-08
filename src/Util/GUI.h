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
    void DrawGridMap(const class SpatialHashGrid& InGrid, int HighlightedCell = -1);

private:
    void SetupStyle();

    std::vector<float> FpsHistory;
    const int MaxHistorySize = 90;
};