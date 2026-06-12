//Copyright 2022 NNTU-CS
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>
#include "tree.h"

static void displayPermutation(const std::vector<char>& perm) {
    for (auto symbol : perm) std::cout << symbol;
    std::cout << "\n";
}

static int64_t getFactorialValue(int num) {
    int64_t resultValue = 1;
    for (int counter = 2; counter <= num; ++counter) {
        resultValue *= counter;
    }
    return resultValue;
}

static std::vector<char> generateSymbolSet(int size) {
    std::vector<char> symbols;
    symbols.reserve(size);
    for (int idx = 0; idx < size; ++idx) {
        symbols.push_back(static_cast<char>('A' + idx));
    }
    return symbols;
}

int main() {
    {
        std::vector<char> testInput = { '1', '2', '3', '4' };
        PMTree testTree(testInput);
        std::cout << "Demo for alphabet {1,2,3,4}\n";
        std::cout << "All permutations:\n";
        
        auto collectedPerms = getAllPerms(testTree);
        for (size_t position = 0; position < collectedPerms.size(); ++position) {
            std::cout << (position + 1) << ": ";
            displayPermutation(collectedPerms[position]);
        }
        std::cout << "\n";
    }
    
    std::cout << "n\tgetAllPerms(ms)\tgetPerm1(ms)\tgetPerm2(ms)\n";
    
    const int minN = 1;
    const int maxN = 15;
    std::mt19937 randomEngine(12345);
    
    for (int currentN = minN; currentN <= maxN; ++currentN) {
        PMTree currentTree(generateSymbolSet(currentN));
        
        int64_t totalPermutationCount = getFactorialValue(currentN);
        std::uniform_int_distribution<int64_t> distribution(1, totalPermutationCount);
        int64_t randomPosition = distribution(randomEngine);
        
        auto startTime = std::chrono::high_resolution_clock::now();
        getAllPerms(currentTree);
        auto endTime = std::chrono::high_resolution_clock::now();
        double timeAllPerms = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        
        startTime = std::chrono::high_resolution_clock::now();
        getPerm1(currentTree, static_cast<int>(randomPosition));
        endTime = std::chrono::high_resolution_clock::now();
        double timePerm1 = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        
        startTime = std::chrono::high_resolution_clock::now();
        getPerm2(currentTree, static_cast<int>(randomPosition));
        endTime = std::chrono::high_resolution_clock::now();
        double timePerm2 = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        
        std::cout << currentN << "\t" 
                  << timeAllPerms << "\t\t" 
                  << timePerm1 << "\t\t" 
                  << timePerm2 << "\n";
    }
    
    return 0;
}
