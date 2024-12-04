#include "visualization/VisualizationManager.hpp"
#include "imgui.h"
#include <algorithm>

VisualizationManager::VisualizationManager()
    : m_speed(1.0f)
    , m_isPaused(true)
    , m_stepMode(false)
{
    m_sortingAlgorithm = std::make_unique<SortingAlgorithm>(100);
}

void VisualizationManager::update() {
    if (!m_isPaused && !m_stepMode) {
        m_sortingAlgorithm->step();
    }
}

void VisualizationManager::render() {
    renderSortingAlgorithm();
}

void VisualizationManager::renderImGui() {
    renderControls();
    renderMetrics();
}

void VisualizationManager::renderSortingAlgorithm() {
    const auto& state = m_sortingAlgorithm->getState();
    
    ImGui::Begin("Algorithm Visualization");
    
    // Calculate dimensions
    const float padding = 10.0f;
    const ImVec2 pos = ImGui::GetCursorScreenPos();
    const ImVec2 availSize = ImGui::GetContentRegionAvail();
    const float width = availSize.x - 2 * padding;
    const float height = availSize.y - 2 * padding;
    
    // Get drawing context
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Draw background
    drawList->AddRectFilled(
        ImVec2(pos.x + padding, pos.y + padding),
        ImVec2(pos.x + width + padding, pos.y + height + padding),
        IM_COL32(50, 50, 50, 255)
    );
    
    // Draw bars
    const float barWidth = width / state.array.size();
    const float maxHeight = height - 20.0f;
    const float maxValue = static_cast<float>(*std::max_element(state.array.begin(), state.array.end()));
    
    for (size_t i = 0; i < state.array.size(); ++i) {
        const float value = static_cast<float>(state.array[i]);
        const float barHeight = (value / maxValue) * maxHeight;
        const float x = pos.x + padding + i * barWidth;
        const float y = pos.y + height + padding;
        
        // Check if this index is highlighted
        bool isHighlighted = std::find(
            state.highlightIndices.begin(),
            state.highlightIndices.end(),
            i
        ) != state.highlightIndices.end();
        
        ImU32 color = isHighlighted ? 
            IM_COL32(255, 100, 100, 255) : 
            IM_COL32(100, 200, 255, 255);
        
        drawList->AddRectFilled(
            ImVec2(x, y),
            ImVec2(x + barWidth - 1, y - barHeight),
            color
        );
    }
    
    ImGui::End();
}

void VisualizationManager::renderControls() {
    ImGui::Begin("Controls");
    
    if (ImGui::Button(m_isPaused ? "Play" : "Pause")) {
        m_isPaused = !m_isPaused;
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Step") && m_isPaused) {
        m_sortingAlgorithm->step();
    }
    
    ImGui::SameLine();
    if (ImGui::Button("Reset")) {
        m_sortingAlgorithm->reset();
        m_isPaused = true;
    }
    
    ImGui::Separator();
    
    const char* algorithms[] = {
        "Quick Sort", "Merge Sort", "Bubble Sort", "Heap Sort"
    };
    static int currentAlgo = 0;
    
    if (ImGui::Combo("Algorithm", &currentAlgo, algorithms, IM_ARRAYSIZE(algorithms))) {
        m_sortingAlgorithm->setAlgorithm(static_cast<SortingAlgorithm::AlgorithmType>(currentAlgo));
        m_sortingAlgorithm->reset();
        m_isPaused = true;
    }
    
    ImGui::SliderFloat("Speed", &m_speed, 0.1f, 5.0f);
    m_sortingAlgorithm->setSpeed(m_speed);
    
    ImGui::End();
}

void VisualizationManager::renderMetrics() {
    ImGui::Begin("Metrics");
    
    const auto& state = m_sortingAlgorithm->getState();
    
    ImGui::Text("Comparisons: %d", state.comparisons);
    ImGui::Text("Swaps: %d", state.swaps);
    ImGui::Text("Time: %.3f s", state.timeElapsed);
    
    ImGui::End();
}
