#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "Rendering/Shader.h"
#include "Rendering/Camera.h"

float Vertices[] = {
    // 위치 (x, y, z)           // 색상 (r, g, b)
    // 뒷면
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

    // 앞면
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

    // 왼쪽면
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

    // 오른쪽면
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,

     // 바닥면
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,

     // 천장면
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
     -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f
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

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // 셰이더 어트리뷰트 설정
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 3. 카메라 생성
    Camera MainCamera(glm::vec3(0.0f, 0.0f, 3.0f));
    float DeltaTime = 0.0f;
    float LastFrame = 0.0f;

    // 4. 렌더링 루프
    while (!glfwWindowShouldClose(Window)) {
        if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(Window, true);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        OurShader.Use();

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

        unsigned int ModelLoc = glGetUniformLocation(OurShader.ID, "model");
        unsigned int ViewLoc = glGetUniformLocation(OurShader.ID, "view");
        unsigned int ProjLoc = glGetUniformLocation(OurShader.ID, "projection");

        glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, glm::value_ptr(Model));
        glUniformMatrix4fv(ViewLoc, 1, GL_FALSE, glm::value_ptr(View));
        glUniformMatrix4fv(ProjLoc, 1, GL_FALSE, glm::value_ptr(Projection));

        // 그리기 (정점 36개)
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();

    return 0;
}