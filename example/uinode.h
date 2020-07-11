#pragma once
#include <memory>
#include <string_view>
#include "graph.h"

namespace example
{

class UiNode
{
    // avoid copy
    UiNode(const UiNode&) = delete;
    UiNode& operator=(const UiNode&) = delete;

    int m_id;
    std::string m_name;

    UiNode(int id, const std::string& name) : m_id(id), m_name(name) {}

public:
    int id() const { return m_id; }
    std::string_view name() const { return m_name; }

    // draw imgui
    void show(Graph<std::shared_ptr<UiNode>>& graph) const;

    // calculate attribute values
    void evaluate(Graph<std::shared_ptr<UiNode>>& graph);

    static std::shared_ptr<UiNode> CreateAdd(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateMultiply(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateOutput(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateSine(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateTime(Graph<std::shared_ptr<UiNode>>& graph);
};

} // namespace example
