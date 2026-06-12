// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

static void traverseAndCollect(PMTree::Node* node, 
                                std::vector<char>& current,
                                std::vector<std::vector<char>>& output,
                                int level, int targetLevel) {
    if (level == targetLevel) {
        output.push_back(current);
        return;
    }
    for (auto& child : node->branches) {
        current.push_back(child->value);
        traverseAndCollect(child, current, output, level + 1, targetLevel);
        current.pop_back();
    }
}

static PMTree::Node* buildSubtree(const std::vector<char>& symbols) {
    PMTree::Node* newNode = new PMTree::Node(0);
    if (symbols.empty()) return newNode;
    
    std::vector<char> sortedSymbols = symbols;
    std::sort(sortedSymbols.begin(), sortedSymbols.end());
    
    for (size_t idx = 0; idx < sortedSymbols.size(); ++idx) {
        char currentSymbol = sortedSymbols[idx];
        std::vector<char> remainingSymbols;
        for (size_t j = 0; j < sortedSymbols.size(); ++j) {
            if (j != idx) remainingSymbols.push_back(sortedSymbols[j]);
        }
        PMTree::Node* childNode = buildSubtree(remainingSymbols);
        childNode->value = currentSymbol;
        newNode->branches.push_back(childNode);
    }
    return newNode;
}

static void deleteSubtree(PMTree::Node* startNode) {
    if (!startNode) return;
    for (auto& childBranch : startNode->branches) {
        deleteSubtree(childBranch);
    }
    delete startNode;
}

PMTree::PMTree(const std::vector<char>& input) : source(input) {
    root = new Node(0);
    std::vector<char> uniqueSymbols = source;
    std::sort(uniqueSymbols.begin(), uniqueSymbols.end());
    uniqueSymbols.erase(std::unique(uniqueSymbols.begin(), uniqueSymbols.end()), 
                        uniqueSymbols.end());
    
    for (char sym : uniqueSymbols) {
        std::vector<char> remaining;
        bool found = false;
        for (char c : source) {
            if (!found && c == sym) {
                found = true;
                continue;
            }
            remaining.push_back(c);
        }
        PMTree::Node* child = buildSubtree(remaining);
        child->value = sym;
        root->branches.push_back(child);
    }
}

PMTree::~PMTree() {
    deleteSubtree(root);
}

PMTree::Node* PMTree::construct(const std::vector<char>& remaining) {
    return buildSubtree(remaining);
}

void PMTree::erase(Node* vertex) {
    deleteSubtree(vertex);
}

std::vector<std::vector<char>> getAllPerms(PMTree& container) {
    std::vector<std::vector<char>> result;
    std::vector<char> buffer;
    int totalLength = static_cast<int>(container.getSource().size());
    
    for (auto& firstLevel : container.getRoot()->branches) {
        buffer.push_back(firstLevel->value);
        traverseAndCollect(firstLevel, buffer, result, 1, totalLength);
        buffer.pop_back();
    }
    return result;
}

static unsigned long long computeFactorial(int n) {
    unsigned long long result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

std::vector<char> getPerm1(PMTree& container, int position) {
    std::vector<std::vector<char>> allPermutations = getAllPerms(container);
    int totalPerms = static_cast<int>(allPermutations.size());
    if (position < 1 || position > totalPerms) return {};
    return allPermutations[position - 1];
}

std::vector<char> getPerm2(PMTree& container, int position) {
    int symbolCount = static_cast<int>(container.getSource().size());
    unsigned long long maxPerms = computeFactorial(symbolCount);
    if (position < 1 || static_cast<unsigned long long>(position) > maxPerms) return {};
    
    std::vector<char> permutation;
    int remainingIndex = position - 1;
    PMTree::Node* currentNode = container.getRoot();
    
    for (int step = 0; step < symbolCount; ++step) {
        unsigned long long blockSize = computeFactorial(symbolCount - step - 1);
        int childIdx = static_cast<int>(remainingIndex / blockSize);
        remainingIndex = remainingIndex % static_cast<int>(blockSize);
        
        if (childIdx >= static_cast<int>(currentNode->branches.size())) return {};
        
        currentNode = currentNode->branches[childIdx];
        permutation.push_back(currentNode->value);
    }
    return permutation;
}
