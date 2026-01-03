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

Camera MainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
bool FirstMouse = true;
float LastX = 800.0f / 2.0f;
float LastY = 600.0f / 2.0f;

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
    // GLFW 초기화
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);
    if (!Window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    // 셰이더 생성
    Shader OurShader("../src/shaders/basic.vert", "../src/shaders/basic.frag");
    OurShader.Use();

    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;

    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 마우스 커서 숨기기
    glfwSetCursorPosCallback(Window, MouseCallback);

    Model Penguin("../resource/Penguin.obj");

    // 렌더링
    while (!glfwWindowShouldClose(Window)) {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MVP 행렬 계산
          // Model
        glm::mat4 Model = glm::mat4(1.0f);

          // View
        float CurrentFrame = glfwGetTime();
        DeltaTime = CurrentFrame - LastFrame;
        LastFrame = CurrentFrame;

        if (glfwGetKey(Window, GLFW_KEY_W) == GLFW_PRESS) MainCamera.ProcessKeyboard("FORWARD", DeltaTime);
        if (glfwGetKey(Window, GLFW_KEY_S) == GLFW_PRESS) MainCamera.ProcessKeyboard("BACKWARD", DeltaTime);
        if (glfwGetKey(Window, GLFW_KEY_A) == GLFW_PRESS) MainCamera.ProcessKeyboard("LEFT", DeltaTime);
        if (glfwGetKey(Window, GLFW_KEY_D) == GLFW_PRESS) MainCamera.ProcessKeyboard("RIGHT", DeltaTime);
        if (glfwGetKey(Window, GLFW_KEY_E) == GLFW_PRESS) MainCamera.ProcessKeyboard("UP", DeltaTime);
        if (glfwGetKey(Window, GLFW_KEY_Q) == GLFW_PRESS) MainCamera.ProcessKeyboard("DOWN", DeltaTime);

        glm::mat4 View = MainCamera.GetViewMatrix();

          // Projection
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        OurShader.SetMat4("projection", Projection);
        OurShader.SetMat4("view", View);

        for (int x = 0; x < 10; x++) {
            for (int z = 0; z < 10; z++) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * 1.0f, 0.0f, z * 1.0f));
                model = glm::scale(model, glm::vec3(0.1f));

                OurShader.SetMat4("model", model);
                Penguin.Draw(OurShader.ID);
            }
        }

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

