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

bool SortingAlgorithm::stepQuickSort() {
    if (m_state.array.empty()) return false;
    
    // Initialize partition range
    if (m_auxArray.empty()) {
        m_auxArray.push_back(0);  // left boundary
        m_auxArray.push_back(m_state.array.size() - 1);  // right boundary
        m_currentIndex = m_auxArray[0];  // pivot index
        m_compareIndex = m_currentIndex + 1;  // scanning index
        m_partitionIndex = m_currentIndex;  // final pivot position
    }
    
    // If we've completed the current partition
    if (m_compareIndex > m_auxArray[1]) {
        // Swap pivot to its final position
        if (m_currentIndex != m_partitionIndex) {
            std::swap(m_state.array[m_currentIndex], m_state.array[m_partitionIndex]);
            m_state.swaps++;
            
            // Update visualization state
            m_state.highlightIndices = {m_currentIndex, m_partitionIndex};
            return true;
        }
        
        // Push sub-partitions onto stack
        int pivotPos = m_partitionIndex;
        if (pivotPos - 1 > m_auxArray[0]) {  // Left partition
            m_auxArray.push_back(m_auxArray[0]);
            m_auxArray.push_back(pivotPos - 1);
        }
        if (pivotPos + 1 < m_auxArray[1]) {  // Right partition
            m_auxArray.push_back(pivotPos + 1);
            m_auxArray.push_back(m_auxArray[1]);
        }
        
        // Pop current partition
        m_auxArray.erase(m_auxArray.begin());
        m_auxArray.erase(m_auxArray.begin());
        
        // If no more partitions, we're done
        if (m_auxArray.empty()) {
            m_finished = true;
            return false;
        }
        
        // Setup next partition
        m_currentIndex = m_auxArray[0];
        m_compareIndex = m_currentIndex + 1;
        m_partitionIndex = m_currentIndex;
        
        // Update visualization state
        m_state.highlightIndices = {m_currentIndex};
        return true;
    }
    
    // Compare current element with pivot
    m_state.comparisons++;
    if (m_state.array[m_compareIndex] < m_state.array[m_currentIndex]) {
        m_partitionIndex++;
        if (m_partitionIndex != m_compareIndex) {
            std::swap(m_state.array[m_partitionIndex], m_state.array[m_compareIndex]);
            m_state.swaps++;
        }
    }
    
    // Update visualization state
    m_state.highlightIndices = {m_currentIndex, m_compareIndex, m_partitionIndex};
    m_compareIndex++;
    return true;
}

// Placeholder implementations for other algorithms
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
