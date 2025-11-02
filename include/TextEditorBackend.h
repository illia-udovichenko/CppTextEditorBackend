#pragma once
#include <iomanip>
#include <memory>
#include "Node.h"

struct TextEditorBackend {
    explicit TextEditorBackend(const std::string &text);

    ~TextEditorBackend();

    size_t size() const;

    size_t lines() const;

    char at(size_t i) const;

    void edit(size_t i, char c);

    void insert(size_t i, char c);

    void erase(size_t i);

    size_t line_start(size_t r) const;

    size_t line_length(size_t r) const;

    size_t char_to_line(size_t i) const;

    void print() const;

private:
    std::shared_ptr<Node> root = nullptr;
    size_t Size;
    size_t Lines = 1;

    void updateNewlines(const std::shared_ptr<Node> &vertex);

    void deleteNode(const std::shared_ptr<Node> &vertex);
};
