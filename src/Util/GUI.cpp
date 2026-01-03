#include "GUI.h"
#include <GLFW/glfw3.h>

GUI::GUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    SetupStyle();
    FpsHistory.resize(MaxHistorySize, 0.0f);
}

GUI::~GUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GUI::NewFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GUI::DrawPerformancePanel(int objectCount) {
    ImGuiIO& io = ImGui::GetIO();

    // FPS 데이터 갱신
    static int offset = 0;
    FpsHistory[offset] = io.Framerate;
    offset = (offset + 1) % MaxHistorySize;

    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);

    ImGui::Begin("Engine Statistics");

    ImGui::Text("FPS: %.1f (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
    ImGui::Text("Total Objects: %d", objectCount);

    ImGui::Separator();
    ImGui::PlotLines("FPS History", FpsHistory.data(), MaxHistorySize, offset, nullptr, 0.0f, 165.0f, ImVec2(0, 80));

    ImGui::End();
}

void GUI::Render() {
    ImGui::Render();

    glDisable(GL_DEPTH_TEST);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glEnable(GL_DEPTH_TEST); // 다음 프레임을 위해 다시 켬
}

void GUI::SetupStyle() {
    ImGui::StyleColorsDark();
}