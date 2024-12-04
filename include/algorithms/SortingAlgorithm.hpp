#pragma once
#include <vector>
#include <string>
#include <functional>

class SortingAlgorithm {
public:
    enum class AlgorithmType {
        QUICK_SORT,
        MERGE_SORT,
        BUBBLE_SORT,
        HEAP_SORT
    };

    struct AlgorithmState {
        std::vector<int> array;
        int comparisons;
        int swaps;
        double timeElapsed;
        std::vector<int> highlightIndices;
    };

    SortingAlgorithm(size_t size = 100);
    
    void reset();
    void shuffle();
    bool step();
    void setSpeed(float speed) { m_speed = speed; }
    void setAlgorithm(AlgorithmType type);
    
    const AlgorithmState& getState() const { return m_state; }
    bool isFinished() const { return m_finished; }
    std::string getAlgorithmName() const;

private:
    void initQuickSort();
    void initMergeSort();
    void initBubbleSort();
    void initHeapSort();

    bool stepQuickSort();
    bool stepMergeSort();
    bool stepBubbleSort();
    bool stepHeapSort();

    AlgorithmState m_state;
    AlgorithmType m_currentAlgorithm;
    float m_speed;
    bool m_finished;
    
    // Algorithm specific state
    std::vector<int> m_auxArray;
    int m_currentIndex;
    int m_compareIndex;
    int m_partitionIndex;
};
