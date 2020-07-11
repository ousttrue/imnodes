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

    int m_id;
    std::string m_name;

    UiNode(int id, const std::string& name) : m_id(id), m_name(name) {}

public:
    int id() const { return m_id; }
    std::string_view name() const { return m_name; }

    // draw imgui
    void show(GraphType& graph) const;

    // calculate attribute values
    void evaluate(GraphType& graph);

    static std::shared_ptr<UiNode> CreateAdd(GraphType& graph);
    static std::shared_ptr<UiNode> CreateMultiply(GraphType& graph);
    static std::shared_ptr<UiNode> CreateOutput(GraphType& graph);
    static std::shared_ptr<UiNode> CreateSine(GraphType& graph);
    static std::shared_ptr<UiNode> CreateTime(GraphType& graph);
};

} // namespace example
