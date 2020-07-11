#pragma once
#include <memory>
#include <string_view>
#include "graph.h"

namespace example
{

class Input
{
    std::string m_name;
    float m_value = 1.0f;

public:
    Input(const std::string& name) : m_name(name) {}
    std::string_view name() const { return m_name; }
    float* ptr() { return &m_value; }
};
using InputPtr = std::shared_ptr<Input>;
class Output
{
    std::string m_name;
    float m_value = 1.0f;

public:
    Output(const std::string& name) : m_name(name) {}
    std::string_view name() const { return m_name; }
    float* ptr() { return &m_value; }
};
using OutputPtr = std::shared_ptr<Output>;

class UiNode;
using UiNodePtr = std::shared_ptr<UiNode>;
using GraphType = Graph<UiNodePtr, InputPtr, OutputPtr>;

class UiNode
{
    // avoid copy
    UiNode(const UiNode&) = delete;
    UiNode& operator=(const UiNode&) = delete;

    std::string m_name;

public:
    UiNode(const std::string& name) : m_name(name) {}
    std::string_view name() const { return m_name; }

    // draw imgui
    void show(GraphType& graph, GraphType::Node &node) const;

    // calculate attribute values
    void evaluate(GraphType& graph, GraphType::Node &node);

    static GraphType::Node& CreateAdd(GraphType& graph);
    static GraphType::Node& CreateMultiply(GraphType& graph);
    static GraphType::Node& CreateOutput(GraphType& graph);
    static GraphType::Node& CreateSine(GraphType& graph);
    static GraphType::Node& CreateTime(GraphType& graph);
};

} // namespace example
