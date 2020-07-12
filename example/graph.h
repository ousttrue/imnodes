#pragma once

#include <stddef.h>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <utility>
#include <vector>
#include <span>
#include <functional>
#include <stdexcept>

namespace example
{

// a very simple directional graph
template<typename NodeData, typename InputData, typename OutputData>
class Graph
{
    static inline int current_id_ = 0;

public:
    struct InputAttribute
    {
        int id;
        InputData data;
        InputAttribute(int id, const InputData& data) : id(id), data(data) {}
    };
    struct OutputAttribute
    {
        int id;
        OutputData data;
        OutputAttribute(int id, const OutputData& data) : id(id), data(data) {}
    };

    struct Node
    {
        int id;
        NodeData data;
        Node(int id, const NodeData& data) : id(id), data(data) {}

        std::vector<InputAttribute> inputs;
        std::vector<OutputAttribute> outputs;

        void add_input(const InputData& input)
        {
            const int id = current_id_++;
            inputs.push_back(InputAttribute(id, input));
        }

        void add_output(const OutputData& output)
        {
            const int id = current_id_++;
            outputs.push_back(OutputAttribute(id, output));
        }
    };

    // These contains map to the node id
    std::unordered_map<int, Node> nodes_;

public:
    struct NodeOutput
    {
        Node& node;
        OutputAttribute& output;
    };
    struct NodeInput
    {
        Node& node;
        InputAttribute& input;
    };

    struct Edge
    {
        int id;
        NodeOutput from;
        NodeInput to;

        Edge(const int id, const NodeOutput& output, const NodeInput& input)
            : id(id), from(output), to(input)
        {
        }

        inline int opposite(const int n) const { return n == from ? to : from; }
        inline bool contains(const int n) const { return n == from.node.id || n == to.node.id; }
    };

    // This container maps to the edge id
    std::unordered_map<int, Edge> edges_;

    // Element access

    Node& node(const int id)
    {
        const auto iter = nodes_.find(id);
        assert(iter != nodes_.end());
        return iter->second;
    }

    NodeInput input_attribute(const int id)
    {
        for (auto& [_, node] : nodes_)
        {
            for (auto& input : node.inputs)
            {
                if (input.id == id)
                {
                    return {node, input};
                }
            }
        }
        throw std::runtime_error("not found");
    }

    NodeOutput output_attribute(const int id)
    {
        for (auto& [_, node] : nodes_)
        {
            for (auto& output : node.outputs)
            {
                if (output.id == id)
                {
                    return {node, output};
                }
            }
        }
        throw std::runtime_error("not found");
    }

    // Modifiers

    Node& insert_node(const NodeData& value)
    {
        const int id = current_id_++;
        auto inserted = nodes_.emplace(id, Node(id, value));
        assert(inserted.second);
        return inserted.first->second;
    }

    void erase_node(const int id)
    {
        // first, remove any potential dangling edges
        {
            static std::vector<int> edges_to_erase;

            for (auto& [_, edge] : edges_)
            {
                if (edge.contains(id))
                {
                    edges_to_erase.push_back(edge.id);
                }
            }

            for (const int edge_id : edges_to_erase)
            {
                erase_edge(edge_id);
            }

            edges_to_erase.clear();
        }

        nodes_.erase(id);
    }

    ///
    /// create connection from attribute between to attribute
    ///
    /// src.from(output) => dst.to(input)
    ///
    /// check validation
    ///
    int insert_edge(const int from_idx, const int to_idx)
    {
        //
        // TODO: avoid loop
        //
        const int id = current_id_++;
        assert(edges_.find(id) == edges_.end());

        auto output = output_attribute(from_idx);
        auto input = input_attribute(to_idx);

        // assert(nodes_.find(from) != nodes_.end());
        // assert(nodes_.find(to) != nodes_.end());
        edges_.insert(std::make_pair(id, Edge(id, output, input)));

        return id;
    }

    void erase_edge(const int edge_id)
    {
        // This is a bit lazy, we find the pointer here, but we refind it when we erase the edge
        // based on id key.
        assert(edges_.find(edge_id) != edges_.end());
        const Edge& edge = edges_.find(edge_id)->second;
        edges_.erase(edge_id);
    }

    Edge* get_edge(const InputAttribute& to)
    {
        for (auto& [id, edge] : edges_)
        {
            if (edge.to.input.id == to.id)
            {
                return &edge;
            }
        }

        return nullptr;
    }
};

} // namespace example
