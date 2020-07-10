#pragma once
#include <memory>
#include "node.h"
#include "graph.h"

namespace example
{

class UiNode
{
public:
    // The identifying id of the ui node. For add, multiply, sine, and time
    // this is the "operation" node id. The additional input nodes are
    // stored in the structs.
    int id;

    static std::shared_ptr<UiNode> CreateAdd(Graph<Node> &graph);
    static std::shared_ptr<UiNode> CreateMultiply(Graph<Node> &graph);
    static std::shared_ptr<UiNode> CreateOutput(Graph<Node> &graph);
    static std::shared_ptr<UiNode> CreateSine(Graph<Node> &graph);
    static std::shared_ptr<UiNode> CreateTime(Graph<Node> &graph);

    virtual void show(Graph<Node> &graph) const = 0;
    virtual bool erase(Graph<Node> &graph) const = 0;
};

} // namespace example
