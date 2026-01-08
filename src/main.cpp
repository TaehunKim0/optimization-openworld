#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Mesh/Model.h"
#include "World/World.h"
#include "Util/GUI.h"
#include "Core/SpatialHashGrid.h"
#include "Core/IGridable.h"

bool bFirstMouse = true;
float LastX = 1920.0f / 2.0f;
float LastY = 1080.0f / 2.0f;
int GridWidth = 200;
int GridHeight = 200;
int CellCount = 10;

Camera MainCamera(glm::vec3(GridWidth/2, 20.0f, GridHeight/2));

void MouseCallback(GLFWwindow* Window, double XPosIn, double YPosIn)
{
    float XPos = static_cast<float>(XPosIn);
    float YPos = static_cast<float>(YPosIn);

    if (bFirstMouse)
    {
        LastX = XPos;
        LastY = YPos;
        bFirstMouse = false;
    }

    float XOffset = XPos - LastX;
    float YOffset = LastY - YPos;

    LastX = XPos;
    LastY = YPos;

    MainCamera.ProcessMouseMovement(XOffset, YOffset);
}

int main() {
    //--- GLFW 檬扁拳
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(1920, 1080, "OpenGL", nullptr, nullptr);
    if (!Window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    //-- 嘉捞歹 积己
    Shader OurShader("../src/shaders/basic.vert", "../src/shaders/basic.frag");
    glEnable(GL_DEPTH_TEST);

    //-- World 积己
    Model Penguin("../resource/Penguin.obj");
    World MyWorld;
    MyWorld.SpawnRandomModels(&Penguin, 1000); 

    SpatialHashGrid Grid(GridWidth, GridHeight, CellCount);
    MyWorld.InitGrid(GridWidth, GridHeight, CellCount);

    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(Window, MouseCallback);

    //-- Imgui 积己
    GUI MyGUI(Window);

    //-- Main Loop --
    while (!glfwWindowShouldClose(Window))
    {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Time 
        float CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        // Grid
        Grid.Clear();
        const auto& Objects = MyWorld.GetObjects();
        for (auto* Obj : Objects)
        {
            Grid.Insert((IGridable*)Obj);
        }

        glm::vec3 camPos = MainCamera.GetPosition();
        int targetCell = Grid.GetCellIndex(camPos.x, camPos.z);

        // Rendering
        MainCamera.UpdateCameraInput(Window, DeltaTime);
        glm::mat4 View = MainCamera.GetViewMatrix();
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);

        OurShader.Use();
        OurShader.SetMat4("projection", Projection);
        OurShader.SetMat4("view", View);

        MyWorld.Render(OurShader);
        MyWorld.RenderGrid(OurShader);

        MyGUI.NewFrame();
        MyGUI.DrawPerformancePanel((int)MyWorld.GetObjects().size());
        MyGUI.DrawGridMap(Grid, targetCell);

        MyGUI.Render();

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

