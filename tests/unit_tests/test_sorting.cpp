#include <gtest/gtest.h>
#include "algorithms/SortingAlgorithm.hpp"

class SortingAlgorithmTest : public ::testing::Test {
protected:
    void SetUp() override {
        sorter = std::make_unique<SortingAlgorithm>(10);
    }
    
    bool isSorted(const std::vector<int>& arr) {
        for (size_t i = 1; i < arr.size(); ++i) {
            if (arr[i] < arr[i-1]) return false;
        }
        return true;
    }
    
    std::unique_ptr<SortingAlgorithm> sorter;
};

TEST_F(SortingAlgorithmTest, BubbleSortCompletes) {
    sorter->setAlgorithm(SortingAlgorithm::AlgorithmType::BUBBLE_SORT);
    
    while (!sorter->isFinished()) {
        sorter->step();
    }
    
    EXPECT_TRUE(isSorted(sorter->getState().array));
}

TEST_F(SortingAlgorithmTest, MetricsAreTracked) {
    sorter->setAlgorithm(SortingAlgorithm::AlgorithmType::BUBBLE_SORT);
    
    const auto& initialState = sorter->getState();
    EXPECT_EQ(initialState.comparisons, 0);
    EXPECT_EQ(initialState.swaps, 0);
    
    sorter->step();
    
    const auto& stateAfterStep = sorter->getState();
    EXPECT_GT(stateAfterStep.comparisons, 0);
}
