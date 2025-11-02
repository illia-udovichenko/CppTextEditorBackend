#include "../include/BSTHelpers.h"
#include "../include/TextEditorBackend.h"
#include <iostream>
#include <memory>

using namespace std;
using namespace BSTHelpers;

// ==================== CONSTRUCTOR / DESTRUCTOR ==================== //

TextEditorBackend::TextEditorBackend(const string &text) {
    // Build initial BST from text
    Size = text.length();
    root = buildNode(text, 0, Size + 1, nullptr); // root = nullptr if text empty
    updateNewlines(root);
    updateHeights(root);
}

TextEditorBackend::~TextEditorBackend() {
    // Properly delete all nodes
    deleteNode(root);
}

// ==================== BASIC GETTERS ==================== //

size_t TextEditorBackend::size() const { return Size; }
size_t TextEditorBackend::lines() const { return Lines; }

// ==================== CHARACTER ACCESS ==================== //

char TextEditorBackend::at(size_t i) const {
    // Return character at position i
    if (i >= Size) throw out_of_range("at");
    return findNode(root, i + 1)->data;
}

// ==================== EDIT OPERATIONS ==================== //

void TextEditorBackend::edit(size_t i, char c) {
    // Replace character at i-position with c
    if (i >= Size) throw out_of_range("edit");
    auto node = findNode(root, i + 1);
    if (node->data == c) return;

    // Update line count if newline is replaced/added
    if (node->data == '\n') {
        Lines--;
        auto tmp = node;
        while (tmp->parent) {
            if (tmp->parent->left == tmp) tmp->parent->newLinesOnLeft--;
            tmp = tmp->parent;
        }
    }
    if (c == '\n') {
        Lines++;
        auto tmp = node;
        while (tmp->parent) {
            if (tmp->parent->left == tmp) tmp->parent->newLinesOnLeft++;
            tmp = tmp->parent;
        }
    }

    node->data = c;
}

void TextEditorBackend::insert(size_t i, char c) {
    // Insert character c after position i
    if (i > Size) throw out_of_range("insert");
    if (c == '\n') Lines++;
    Size++;

    // Handle empty tree
    if (Size == 1) {
        root = make_shared<Node>(c, 1, nullptr);
        return;
    }

    // Insert at the end
    if (i == Size - 1) {
        shared_ptr<Node> lastNode = findNode(root, i);
        lastNode->right = make_shared<Node>(c, 1, lastNode);
        return;
    }

    // Insert between existing nodes
    shared_ptr<Node> prevNode = incFindNode(root, i + 1, (c == '\n'));
    char prevData = prevNode->data;
    insertNode(prevNode, (prevData == '\n'), prevData);
    prevNode->data = c;
}

void TextEditorBackend::erase(size_t i) {
    // Erase character at position i
    if (i >= Size) throw out_of_range("erase");

    shared_ptr<Node> node = decFindNode(root, i + 1);

    // Adjust newline count
    if (node->data == '\n') {
        newLineDec(node);
        Lines--;
    }
    Size--;

    // If node has 2 children → find successor
    if (node->left && node->right) {
        shared_ptr<Node> vertex = node->right;
        vertex->nodesOnLeft--;
        while (vertex->left) {
            vertex = vertex->left;
            vertex->nodesOnLeft--;
        }

        // Update newline counters for path
        if (vertex->data == '\n') {
            shared_ptr<Node> tmp = vertex;
            while (tmp != node) {
                tmp->newLinesOnLeft--;
                tmp = tmp->parent;
            }
        }

        node->data = vertex->data;
        deleteNode(vertex); // remove successor
    } else
        deleteNode(node); // simple case
}

// ==================== LINE-BASED OPERATIONS ==================== //

size_t TextEditorBackend::line_start(size_t r) const {
    // Return index of first char in r-th line
    if (r >= Lines)
        throw out_of_range("Line should be in interval [0, lines())");
    if (r == 0) return 0;
    return findLineIdx(root, r, 0);
}

size_t TextEditorBackend::line_length(size_t r) const {
    // Return number of characters in r-th line
    if (r >= Lines)
        throw out_of_range("Line should be in interval [0, lines())");

    size_t startNewLine = (r == 0) ? 0 : findLineIdx(root, r, 0);
    size_t endNewLine = (r == Lines - 1) ? Size : findLineIdx(root, r + 1, 0);
    return endNewLine - startNewLine;
}

size_t TextEditorBackend::char_to_line(size_t i) const {
    // Return line index that contains character at i
    if (i >= Size) throw out_of_range("char_to_line");
    return countNewLines(root, i + 1, 0);
}

// ==================== DEBUG / DISPLAY ==================== //

void TextEditorBackend::print() const {
    // Print the whole text (in-order traversal)
    showNode(root);
}

// ==================== PRIVATE HELPERS ==================== //

void TextEditorBackend::updateNewlines(const shared_ptr<Node> &vertex) {
    // Recursively count and mark newline nodes
    if (!vertex) return;
    if (vertex->data == '\n') {
        newLineInc(vertex);
        Lines++;
    }
    updateNewlines(vertex->left);
    updateNewlines(vertex->right);
}

void TextEditorBackend::deleteNode(const shared_ptr<Node> &vertex) {
    // Remove node from tree and reconnect children properly
    shared_ptr<Node> &parent = vertex->parent;
    shared_ptr<Node> &son = vertex->left ? vertex->left : vertex->right;

    if (!son) {
        // No children
        if (vertex != root) {
            parent->left == vertex ? parent->left = nullptr : parent->right = nullptr;
            parent = nullptr;
        } else
            root = nullptr;
    } else if (vertex == root) {
        // Root with one child
        root = son;
        vertex->left ? vertex->left = nullptr : vertex->right = nullptr;
        son->parent = nullptr;
    } else {
        // One child and not root
        parent->left == vertex ? parent->left = son : parent->right = son;
        son->parent = parent;

        vertex->left ? vertex->left = nullptr : vertex->right = nullptr;
        vertex->parent = nullptr;
    }
}
