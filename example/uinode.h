#pragma once
#include <memory>
#include <string_view>
#include <limits>
#include <functional>
#include "graph.h"

namespace example
{

class Pin
{
    std::string m_name;
    float m_value = std::numeric_limits<float>::quiet_NaN();

public:
    Pin(const std::string& name) : m_name(name) {}
    std::string_view name() const { return m_name; }
    float& value() { return m_value; }
    void value(float n) { m_value = n; }
};
using InputPtr = std::shared_ptr<Pin>;
using OutputPtr = std::shared_ptr<Pin>;

class UiNode;
using UiNodePtr = std::shared_ptr<UiNode>;
using GraphType = Graph<UiNodePtr, InputPtr, OutputPtr>;

class UiNode
{
    // avoid copy
    UiNode(const UiNode&) = delete;
    UiNode& operator=(const UiNode&) = delete;

    std::string m_name;

    using Operation = std::function<void()>;
    Operation m_operation;

    uint32_t m_lastFrame = -1;

public:
    UiNode(const std::string& name, const Operation& operation)
        : m_name(name), m_operation(operation)
    {
    }
    std::string_view name() const { return m_name; }

    // draw imgui
    void show(GraphType& graph, GraphType::Node& node) const;

    // calculate attribute values
    void evaluate(GraphType& graph, GraphType::Node& node);

    // update node
    void update();

    static GraphType::Node& CreateAdd(GraphType& graph);
    static GraphType::Node& CreateMultiply(GraphType& graph);
    static GraphType::Node& CreateOutput(GraphType& graph);
    static GraphType::Node& CreateSine(GraphType& graph);
    static GraphType::Node& CreateTime(GraphType& graph);
};

} // namespace example
