#pragma once
#include "algorithms/SortingAlgorithm.hpp"
#include <memory>

class VisualizationManager {
public:
    VisualizationManager();
    
    void update();
    void render();
    void renderImGui();

private:
    void renderSortingAlgorithm();
    void renderControls();
    void renderMetrics();

    std::unique_ptr<SortingAlgorithm> m_sortingAlgorithm;
    float m_speed;
    bool m_isPaused;
    bool m_stepMode;
};
