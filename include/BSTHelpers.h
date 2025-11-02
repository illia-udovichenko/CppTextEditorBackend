#pragma once
#include <iomanip>
#include <memory>
#include "../include/Node.h"

namespace BSTHelpers {
    std::shared_ptr<Node> buildNode(const std::string &text, size_t left, size_t right, const std::shared_ptr<Node> &parent);

    void showNode(const std::shared_ptr<Node> &node);

    void deleteNode(const std::shared_ptr<Node> &node);

    void insertNode(std::shared_ptr<Node> &node, size_t newLine, char data);

    std::shared_ptr<Node> findNode(const std::shared_ptr<Node> &node, size_t index);

    std::shared_ptr<Node> incFindNode(std::shared_ptr<Node> &node, size_t index, size_t newLine);

    std::shared_ptr<Node> decFindNode(std::shared_ptr<Node> &node, size_t index);

    void calculateHeight(const std::shared_ptr<Node> &node);

    void updateHeights(const std::shared_ptr<Node> &node);

    void newLineInc(const std::shared_ptr<Node> &node);

    void newLineDec(const std::shared_ptr<Node> &node);

    size_t findLineIdx(const std::shared_ptr<Node> &node, size_t line, size_t index);

    size_t countNewLines(const std::shared_ptr<Node> &node, size_t index, size_t counter);
}
