#pragma once

#include <memory>
#include <vector>

namespace ra::module::calculator {

class SyntaxNode {
public:
    SyntaxNode() = default;
    virtual ~SyntaxNode() = default;

    SyntaxNode(const SyntaxNode&) = delete;
    SyntaxNode& operator=(const SyntaxNode&) = delete;

    std::vector<std::shared_ptr<SyntaxNode>> children;
};

}