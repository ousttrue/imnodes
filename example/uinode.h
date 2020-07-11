#pragma once
#include <memory>
#include <string_view>
#include "graph.h"

namespace example
{

class UiNode
{
    // The identifying id of the ui node. For add, multiply, sine, and time
    // this is the "operation" node id. The additional input nodes are
    // stored in the structs.
    int m_id;

    std::string m_name;

    // avoid copy
    UiNode(const UiNode&) = delete;
    UiNode& operator=(const UiNode&) = delete;

protected:
    UiNode(int id, const std::string& name) : m_id(id), m_name(name) {}

public:
    int id() const { return m_id; }
    std::string_view name() const { return m_name; }

    static std::shared_ptr<UiNode> CreateAdd(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateMultiply(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateOutput(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateSine(Graph<std::shared_ptr<UiNode>>& graph);
    static std::shared_ptr<UiNode> CreateTime(Graph<std::shared_ptr<UiNode>>& graph);

    virtual void show(Graph<std::shared_ptr<UiNode>>& graph) const;
};

} // namespace example
