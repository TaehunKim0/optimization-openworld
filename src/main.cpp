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

Camera MainCamera(glm::vec3(0.0f, 20.0f, -15.0f));
bool FirstMouse = true;
float LastX = 1920.0f / 2.0f;
float LastY = 1080.0f / 2.0f;

void MouseCallback(GLFWwindow* Window, double XPosIn, double YPosIn)
{
    float XPos = static_cast<float>(XPosIn);
    float YPos = static_cast<float>(YPosIn);

    if (FirstMouse) {
        LastX = XPos;
        LastY = YPos;
        FirstMouse = false;
    }

    float XOffset = XPos - LastX;
    float YOffset = LastY - YPos; // Y좌표는 아래에서 위로 증가하므로 반전

    LastX = XPos;
    LastY = YPos;

    MainCamera.ProcessMouseMovement(XOffset, YOffset);
}

int main() {
    //--- GLFW 초기화
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


    //-- 셰이더 생성
    Shader OurShader("../src/shaders/basic.vert", "../src/shaders/basic.frag");
    glEnable(GL_DEPTH_TEST);

    //-- World 생성
    Model Penguin("../resource/Penguin.obj");
    World MyWorld;
    MyWorld.SpawnRandomModels(&Penguin, 1000); 

    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 마우스 커서 숨기기
    glfwSetCursorPosCallback(Window, MouseCallback);

    GUI MyGUI(Window);


    //-- 렌더링 --
    while (!glfwWindowShouldClose(Window)) {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        MainCamera.UpdateCameraInput(Window, DeltaTime);
        glm::mat4 View = MainCamera.GetViewMatrix();
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        OurShader.Use();
        OurShader.SetMat4("projection", Projection);
        OurShader.SetMat4("view", View);

        MyWorld.Render(OurShader);

        MyGUI.NewFrame();
        MyGUI.DrawPerformancePanel((int)MyWorld.GetObjects().size());
        MyGUI.Render();

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

