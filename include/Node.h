#pragma once
#include <memory>
#include <algorithm>
#include <bitset>

struct Node {
    char data;
    size_t nodesOnLeft; // current node also included
    size_t newLinesOnLeft = 0; // current \n not included
    size_t height = 0;
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
    std::shared_ptr<Node> parent = nullptr;

    Node(char value, size_t number, std::shared_ptr<Node> father) : data(value), nodesOnLeft(number),
                                                                    parent(std::move(father)) {
    }

    void del() {
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};
