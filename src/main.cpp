#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "Rendering/Shader.h"
#include "Rendering/Camera.h"
#include "Mesh/Mesh.h"

std::vector<Vertex> CubeVertices = {
    // 위치(Position)              // 색상(Color - RGB)
    // 뒤쪽 면 - 빨간색
    {glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1.0f, 0.0f, 0.0f)},

    // 앞쪽 면 - 주황색
    {glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1.0f, 0.5f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(1.0f, 0.5f, 0.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 0.5f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 0.5f, 0.0f)},

    // 왼쪽 면 - 노란색
    {glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 0.0f)},
    {glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(1.0f, 1.0f, 0.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(1.0f, 1.0f, 0.0f)},

    // 오른쪽 면 - 초록색
    {glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, 1.0f, 0.0f)},

    // 아래쪽 면 - 파란색
    {glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)},
    {glm::vec3(0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)},
    {glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec3(0.0f, 0.0f, 1.0f)},

    // 위쪽 면 - 보라색
    {glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.5f, 0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.5f, 0.0f, 1.0f)},
    {glm::vec3(0.5f,  0.5f,  0.5f),  glm::vec3(0.5f, 0.0f, 1.0f)},
    {glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec3(0.5f, 0.0f, 1.0f)}
};

std::vector<unsigned int> CubeIndices = {
    0, 1, 2, 2, 3, 0,       // 뒤
    4, 5, 6, 6, 7, 4,       // 앞
    8, 9, 10, 10, 11, 8,    // 왼쪽
    12, 13, 14, 14, 15, 12, // 오른쪽
    16, 17, 18, 18, 19, 16, // 아래
    20, 21, 22, 22, 23, 20  // 위
};

int main() {
    // 1. GLFW 초기화
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);
    if (!Window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    glEnable(GL_DEPTH_TEST);

    // 2. 셰이더 생성
    Shader OurShader("../src/shaders/basic.vert", "../src/shaders/basic.frag");
    OurShader.Use();

    // 3. 카메라 생성
    Camera MainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;

    Mesh CubeMesh(CubeVertices, CubeIndices);


    // 4. 렌더링 루프
    while (!glfwWindowShouldClose(Window)) {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // MVP 행렬 계산
          // Model
        glm::mat4 Model = glm::mat4(1.0f);
        Model = glm::rotate(Model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));

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
        OurShader.SetMat4("model", Model);
        OurShader.SetMat4("view", View);

        CubeMesh.Draw();

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}