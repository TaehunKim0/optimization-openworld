#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Rendering/Shader.h"

int main() {
    // 1. GLFW 초기화 및 윈도우 생성
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* Window = glfwCreateWindow(800, 600, "Day 2 - Triangle", nullptr, nullptr);
    if (!Window) {
        std::cout << "Window 생성 실패" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // 2. 셰이더 설정
    Shader OurShader("../src/shaders/basic.vert", "../src/shaders/basic.frag");

    float Vertices[] = {
     0.5f,  0.5f, 0.0f,  // 우측 상단 (0번)
     0.5f, -0.5f, 0.0f,  // 우측 하단 (1번)
    -0.5f, -0.5f, 0.0f,  // 좌측 하단 (2번)
    -0.5f,  0.5f, 0.0f   // 좌측 상단 (3번)
    };

    unsigned int Indices[] = {
        0, 1, 3, // 첫 번째 삼각형
        1, 2, 3  // 두 번째 삼각형
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // VAO
    glBindVertexArray(VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // GPU에게 데이터를 어떻게 해석할지 알려줌 (Location 0, 크기 3, 타입 Float)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 

    // 3. 렌더링
    while (!glfwWindowShouldClose(Window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        OurShader.Use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // 4. 리소스 정리
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}