#include "algorithms/SortingAlgorithm.hpp"
#include <random>
#include <algorithm>
#include <chrono>

SortingAlgorithm::SortingAlgorithm(size_t size) 
    : m_speed(1.0f)
    , m_finished(false)
    , m_currentIndex(0)
    , m_compareIndex(0)
    , m_partitionIndex(0)
{
    m_state.array.resize(size);
    reset();
    setAlgorithm(AlgorithmType::QUICK_SORT);
}

void SortingAlgorithm::reset() {
    for (size_t i = 0; i < m_state.array.size(); ++i) {
        m_state.array[i] = static_cast<int>(i);
    }
    m_state.comparisons = 0;
    m_state.swaps = 0;
    m_state.timeElapsed = 0;
    m_state.highlightIndices.clear();
    shuffle();
}

void SortingAlgorithm::shuffle() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(m_state.array.begin(), m_state.array.end(), gen);
    m_finished = false;
}

void SortingAlgorithm::setAlgorithm(AlgorithmType type) {
    m_currentAlgorithm = type;
    m_finished = false;
    m_currentIndex = 0;
    m_compareIndex = 0;
    m_partitionIndex = 0;
    
    switch (type) {
        case AlgorithmType::QUICK_SORT:
            initQuickSort();
            break;
        case AlgorithmType::MERGE_SORT:
            initMergeSort();
            break;
        case AlgorithmType::BUBBLE_SORT:
            initBubbleSort();
            break;
        case AlgorithmType::HEAP_SORT:
            initHeapSort();
            break;
    }
}

bool SortingAlgorithm::step() {
    if (m_finished) return false;

    auto start = std::chrono::high_resolution_clock::now();
    
    bool result = false;
    switch (m_currentAlgorithm) {
        case AlgorithmType::QUICK_SORT:
            result = stepQuickSort();
            break;
        case AlgorithmType::MERGE_SORT:
            result = stepMergeSort();
            break;
        case AlgorithmType::BUBBLE_SORT:
            result = stepBubbleSort();
            break;
        case AlgorithmType::HEAP_SORT:
            result = stepHeapSort();
            break;
    }

    auto end = std::chrono::high_resolution_clock::now();
    m_state.timeElapsed += std::chrono::duration<double>(end - start).count();
    
    return result;
}

std::string SortingAlgorithm::getAlgorithmName() const {
    switch (m_currentAlgorithm) {
        case AlgorithmType::QUICK_SORT: return "Quick Sort";
        case AlgorithmType::MERGE_SORT: return "Merge Sort";
        case AlgorithmType::BUBBLE_SORT: return "Bubble Sort";
        case AlgorithmType::HEAP_SORT: return "Heap Sort";
        default: return "Unknown";
    }
}

// Initialize algorithms
void SortingAlgorithm::initQuickSort() {
    m_auxArray = m_state.array;
}

void SortingAlgorithm::initMergeSort() {
    m_auxArray.resize(m_state.array.size());
}

void SortingAlgorithm::initBubbleSort() {
    m_currentIndex = 0;
    m_compareIndex = 0;
}

void SortingAlgorithm::initHeapSort() {
    m_currentIndex = m_state.array.size() / 2 - 1;
}

// Step implementations
bool SortingAlgorithm::stepBubbleSort() {
    if (m_currentIndex >= m_state.array.size() - 1) {
        m_finished = true;
        return false;
    }

    m_state.highlightIndices = {m_currentIndex, m_currentIndex + 1};
    
    if (m_state.array[m_currentIndex] > m_state.array[m_currentIndex + 1]) {
        std::swap(m_state.array[m_currentIndex], m_state.array[m_currentIndex + 1]);
        m_state.swaps++;
    }
    m_state.comparisons++;
    
    m_currentIndex++;
    if (m_currentIndex >= m_state.array.size() - m_compareIndex - 1) {
        m_currentIndex = 0;
        m_compareIndex++;
    }
    
    return true;
}

// Placeholder implementations for other algorithms
bool SortingAlgorithm::stepQuickSort() {
    // TODO: Implement quick sort step
    m_finished = true;
    return false;
}

bool SortingAlgorithm::stepMergeSort() {
    // TODO: Implement merge sort step
    m_finished = true;
    return false;
}

bool SortingAlgorithm::stepHeapSort() {
    // TODO: Implement heap sort step
    m_finished = true;
    return false;
}
