#include <iomanip>
#include <iostream>
#include <memory>
#include <algorithm>
#include <bitset>
#include "../include/BSTHelpers.h"

using namespace std;

namespace BSTHelpers {
    // Builds a balanced BST (Binary Search Tree) from text recursively.
    // Each node stores its character and subtree info
    shared_ptr<Node> buildNode(const string &text, size_t left, size_t right, const shared_ptr<Node> &parent) {
        if (right - left < 2)
            return nullptr;

        size_t mid = (left + right) / 2;
        auto node = make_shared<Node>(text[mid - 1], mid - left, parent);
        node->left = buildNode(text, left, mid, node);
        node->right = buildNode(text, mid, right, node);

        return node;
    }

    // Prints all characters from the tree in-order (left-root-right)
    void showNode(const shared_ptr<Node> &node) {
        if (!node)
            return;
        showNode(node->left);
        cout << node->data;
        showNode(node->right);
    }

    // Recursively deletes all nodes and their children
    void deleteNode(const shared_ptr<Node> &node) {
        if (!node)
            return;
        deleteNode(node->left);
        deleteNode(node->right);
        node->del();
    }

    // Inserts a new node after a given node (successor logic).
    // Updates subtree counts for nodes along the insertion path
    void insertNode(shared_ptr<Node> &node, size_t newLine, char data) {
        // Find free place to insert previous node (algorithm of successor)
        // Then create node with previous data
        shared_ptr<Node> son = node->right;
        if (son)
            while (son) {
                son->nodesOnLeft++;
                son->newLinesOnLeft += newLine;
                if (!son->left) {
                    son->left = make_shared<Node>(data, 1, son);
                    return;
                }
                son = son->left;
            }
        else
            node->right = make_shared<Node>(data, 1, node);
    }

    // Finds a node by its index in-order
    shared_ptr<Node> findNode(const shared_ptr<Node> &node, size_t index) {
        if (index == node->nodesOnLeft)
            return node;
        if (index < node->nodesOnLeft)
            return findNode(node->left, index);
        return findNode(node->right, index - node->nodesOnLeft);
    }

    // Same as findNode, but increments line/character counters along the path (used after insertion)
    shared_ptr<Node> incFindNode(shared_ptr<Node> &node, size_t index, size_t newLine) {
        // Increment for parent if its parent's left son
        if (node->parent && node->parent->left == node) {
            node->parent->nodesOnLeft++;
            node->parent->newLinesOnLeft += newLine;
        }
        // Same logic as in findBVS
        if (index == node->nodesOnLeft)
            return node;
        if (index < node->nodesOnLeft)
            return incFindNode(node->left, index, newLine);
        return incFindNode(node->right, index - node->nodesOnLeft, newLine);
    }

    // Same as findNode, but decrements subtree counters along the path (used after deletion)
    shared_ptr<Node> decFindNode(shared_ptr<Node> &node, size_t index) {
        // Decrement for parent if its parent's left son
        if (node->parent && node->parent->left == node)
            node->parent->nodesOnLeft--;
        // Same logic as in findBVS
        if (index == node->nodesOnLeft)
            return node;
        if (index < node->nodesOnLeft)
            return decFindNode(node->left, index);
        return decFindNode(node->right, index - node->nodesOnLeft);
    }

    // Recomputes node height based on its children
    void calculateHeight(const shared_ptr<Node> &node) {
        size_t left = node->left ? node->left->height : 0;
        size_t right = node->right ? node->right->height : 0;
        node->height = max(left, right) + 1;
    }

    // Updates heights of all nodes in the tree (post-order traversal)
    void updateHeights(const shared_ptr<Node> &node) {
        if (!node)
            return;
        updateHeights(node->left);
        updateHeights(node->right);
        calculateHeight(node);
    }

    // Increments newline counters for all parent nodes up to the root
    void newLineInc(const shared_ptr<Node> &node) {
        // Increment for parent if its parent's left son
        if (node->parent && node->parent->left == node)
            node->parent->newLinesOnLeft++;
        if (node->parent)
            newLineInc(node->parent);
    }

    // Decrements newline counters for all parent nodes up to the root
    void newLineDec(const shared_ptr<Node> &node) {
        // Decrement for parent if its parent's left son
        if (node->parent && node->parent->left == node)
            node->parent->newLinesOnLeft--;
        if (node->parent)
            newLineDec(node->parent);
    }

    // Finds the absolute character index (0-based) of the start of a specific line
    size_t findLineIdx(const shared_ptr<Node> &node, size_t line, size_t index) {
        size_t newLine = node->data == '\n';
        if (line == node->newLinesOnLeft + 1 && newLine)
            return index + node->nodesOnLeft;
        if (line <= node->newLinesOnLeft)
            return findLineIdx(node->left, line, index);
        return findLineIdx(node->right, line - node->newLinesOnLeft - newLine, index + node->nodesOnLeft);
    }

    // Counts how many newline characters exist before a given index
    size_t countNewLines(const shared_ptr<Node> &node, size_t index, size_t counter) {
        if (index == node->nodesOnLeft)
            return counter + node->newLinesOnLeft;
        if (index < node->nodesOnLeft)
            return countNewLines(node->left, index, counter);
        return countNewLines(node->right, index - node->nodesOnLeft,
                             counter + node->newLinesOnLeft + (node->data == '\n'));
    }
}
