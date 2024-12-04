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
    const float height = availSize.y - 2 * padding - 30.0f; // Leave space for legend
    
    // Get drawing context
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    
    // Draw background
    drawList->AddRectFilled(
        ImVec2(pos.x + padding, pos.y + padding),
        ImVec2(pos.x + width + padding, pos.y + height + padding),
        IM_COL32(30, 30, 30, 255)
    );
    
    // Draw grid lines
    const int gridLines = 10;
    const float gridSpacing = height / gridLines;
    for (int i = 1; i < gridLines; i++) {
        float y = pos.y + padding + i * gridSpacing;
        drawList->AddLine(
            ImVec2(pos.x + padding, y),
            ImVec2(pos.x + width + padding, y),
            IM_COL32(50, 50, 50, 255)
        );
    }
    
    // Draw bars
    const float barWidth = width / state.array.size();
    const float maxHeight = height - 20.0f;
    const float maxValue = static_cast<float>(*std::max_element(state.array.begin(), state.array.end()));
    
    for (size_t i = 0; i < state.array.size(); ++i) {
        const float value = static_cast<float>(state.array[i]);
        const float barHeight = (value / maxValue) * maxHeight;
        const float x = pos.x + padding + i * barWidth;
        const float y = pos.y + height + padding;
        
        // Determine bar color based on its role
        ImU32 color;
        if (std::find(state.highlightIndices.begin(), state.highlightIndices.end(), i) != state.highlightIndices.end()) {
            if (i == m_sortingAlgorithm->getCurrentIndex()) {
                color = IM_COL32(255, 100, 100, 255); // Pivot/Current element (red)
            } else if (i == m_sortingAlgorithm->getCompareIndex()) {
                color = IM_COL32(100, 255, 100, 255); // Compare element (green)
            } else {
                color = IM_COL32(255, 200, 100, 255); // Other highlighted elements (orange)
            }
        } else {
            color = IM_COL32(100, 150, 255, 255); // Normal elements (blue)
        }
        
        // Draw bar
        drawList->AddRectFilled(
            ImVec2(x, y),
            ImVec2(x + barWidth - 1, y - barHeight),
            color
        );
    }
    
    // Draw legend
    const float legendY = pos.y + height + padding + 10.0f;
    const float legendX = pos.x + padding;
    const float legendSpacing = 150.0f;
    
    // Pivot element
    drawList->AddRectFilled(
        ImVec2(legendX, legendY),
        ImVec2(legendX + 20, legendY + 20),
        IM_COL32(255, 100, 100, 255)
    );
    drawList->AddText(
        ImVec2(legendX + 25, legendY),
        IM_COL32(255, 255, 255, 255),
        "Pivot/Current"
    );
    
    // Compare element
    drawList->AddRectFilled(
        ImVec2(legendX + legendSpacing, legendY),
        ImVec2(legendX + legendSpacing + 20, legendY + 20),
        IM_COL32(100, 255, 100, 255)
    );
    drawList->AddText(
        ImVec2(legendX + legendSpacing + 25, legendY),
        IM_COL32(255, 255, 255, 255),
        "Compare"
    );
    
    // Other highlighted
    drawList->AddRectFilled(
        ImVec2(legendX + legendSpacing * 2, legendY),
        ImVec2(legendX + legendSpacing * 2 + 20, legendY + 20),
        IM_COL32(255, 200, 100, 255)
    );
    drawList->AddText(
        ImVec2(legendX + legendSpacing * 2 + 25, legendY),
        IM_COL32(255, 255, 255, 255),
        "Partition"
    );
    
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
    
    // Algorithm Info
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Algorithm: %s", m_sortingAlgorithm->getAlgorithmName().c_str());
    ImGui::Separator();
    
    // Performance Metrics
    ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Performance Metrics:");
    ImGui::Text("Comparisons: %d", state.comparisons);
    ImGui::Text("Swaps: %d", state.swaps);
    ImGui::Text("Time: %.3f s", state.timeElapsed);
    
    // Array Info
    ImGui::Separator();
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 1.0f, 1.0f), "Array Information:");
    ImGui::Text("Size: %zu elements", state.array.size());
    
    // Current State
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "Current State:");
    ImGui::Text("Current Index: %d", m_sortingAlgorithm->getCurrentIndex());
    ImGui::Text("Compare Index: %d", m_sortingAlgorithm->getCompareIndex());
    ImGui::Text("Partition Index: %d", m_sortingAlgorithm->getPartitionIndex());
    
    // Status
    ImGui::Separator();
    if (m_sortingAlgorithm->isFinished()) {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Status: Completed");
    } else if (m_isPaused) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Status: Paused");
    } else {
        ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "Status: Running");
    }
    
    // Theoretical Complexity
    ImGui::Separator();
    ImGui::TextColored(ImVec4(1.0f, 0.5f, 1.0f, 1.0f), "Theoretical Complexity:");
    switch (m_sortingAlgorithm->getAlgorithmType()) {
        case SortingAlgorithm::AlgorithmType::QUICK_SORT:
            ImGui::Text("Average: O(n log n)");
            ImGui::Text("Worst: O(n²)");
            break;
        case SortingAlgorithm::AlgorithmType::MERGE_SORT:
            ImGui::Text("Average: O(n log n)");
            ImGui::Text("Worst: O(n log n)");
            break;
        case SortingAlgorithm::AlgorithmType::BUBBLE_SORT:
            ImGui::Text("Average: O(n²)");
            ImGui::Text("Worst: O(n²)");
            break;
        case SortingAlgorithm::AlgorithmType::HEAP_SORT:
            ImGui::Text("Average: O(n log n)");
            ImGui::Text("Worst: O(n log n)");
            break;
    }
    
    ImGui::End();
}
