#include "GUI.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Core/SpatialHashGrid.h"

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

void GUI::DrawGridMap(const SpatialHashGrid& InGrid, int InHighlightedCell)
{
    ImGui::SetNextWindowPos(ImVec2(1650, 30), ImGuiCond_FirstUseEver);
    ImGui::Begin("Spatial Hash Grid Map");

    ImDrawList* DrawList = ImGui::GetWindowDrawList();
    ImVec2 CanvasPos = ImGui::GetCursorScreenPos();

    ImVec2 CanvasSize = ImVec2(200.0f, 200.0f);
    ImGui::InvisibleButton("GridCanvas", CanvasSize);

    DrawList->AddRectFilled(CanvasPos,
        ImVec2(CanvasPos.x + CanvasSize.x, CanvasPos.y + CanvasSize.y),
        IM_COL32(10, 10, 10, 255));

    int GridCols = InGrid.GetCols();
    int GridRows = InGrid.GetRows();

    // 캔버스 크기를 칸 수로 나누어 픽셀 단위 셀 크기 계산
    float CellPixelWidth = CanvasSize.x / static_cast<float>(GridCols);
    float CellPixelHeight = CanvasSize.y / static_cast<float>(GridRows);

    for (int R = 0; R < GridRows; R++)
    {
        for (int C = 0; C < GridCols; C++)
        {
            // 행 우선 인덱스 계산
            int CurrentIdx = R * GridCols + C;

            // 해당 셀의 객체 리스트 참조 (const 유지)
            const auto& Objects = const_cast<SpatialHashGrid&>(InGrid).GetGridablesInCell(CurrentIdx);
            size_t ObjectCount = Objects.size();

            // 셀의 화면 좌표 계산
            ImVec2 PMin = ImVec2(CanvasPos.x + C * CellPixelWidth, CanvasPos.y + R * CellPixelHeight);
            ImVec2 PMax = ImVec2(PMin.x + CellPixelWidth, PMin.y + CellPixelHeight);

            // 1. 하이라이트 셀 (노란색)
            if (CurrentIdx == InHighlightedCell)
            {
                DrawList->AddRectFilled(PMin, PMax, IM_COL32(255, 255, 0, 200));
            }
            // 2. 객체가 존재하는 셀 (빨간색 농도 조절)
            else if (ObjectCount > 0)
            {
                // 최대 10마리 기준으로 농도 결정
                float Intensity = glm::min(static_cast<float>(ObjectCount) / 10.0f, 1.0f);
                int RedValue = static_cast<int>(Intensity * 200.0f) + 55; // 55 ~ 255

                DrawList->AddRectFilled(PMin, PMax, IM_COL32(RedValue, 0, 0, 255));
            }

            // 3. 격자 가이드선 (선택 사항)
            DrawList->AddRect(PMin, PMax, IM_COL32(255, 255, 255, 15));
        }
    }
    ImGui::End();
}

void GUI::Render() {
    ImGui::Render();

    glDisable(GL_DEPTH_TEST);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glEnable(GL_DEPTH_TEST);
}

void GUI::SetupStyle() {
    ImGui::StyleColorsDark();
}