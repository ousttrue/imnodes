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

        std::vector<int> neighbors;
        std::vector<InputAttribute> inputs;
        std::vector<OutputAttribute> outputs;

        void erase_edge(int to_node)
        {
            auto iter = std::find(neighbors.begin(), neighbors.end(), to_node);
            assert(iter != neighbors.end());
            neighbors.erase(iter);
        }

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
    struct Edge
    {
        int id;
        int from, to;

        Edge() = default;
        Edge(const int id, const int f, const int t) : id(id), from(f), to(t) {}

        inline int opposite(const int n) const { return n == from ? to : from; }
        inline bool contains(const int n) const { return n == from || n == to; }
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

    struct NodeOutput
    {
        Node& node;
        OutputAttribute& output;
    };
    NodeOutput edge_output(int id)
    {
        for (auto& [k, v] : nodes_)
        {
            for (auto& output : v.outputs)
            {
                if (output.id == id)
                {
                    return NodeOutput{v, output};
                }
            }
        }

        throw std::runtime_error("not found");
    }

    struct NodeInput
    {
        Node& node;
        InputAttribute& input;
    };
    NodeInput edge_input(int id)
    {
        for (auto& [k, v] : nodes_)
        {
            for (auto& input : v.inputs)
            {
                if (input.id == id)
                {
                    return NodeInput{v, input};
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
    /// check validation
    ///
    int insert_edge(const int from_idx, const int to_idx)
    {
        //
        // TODO: avoid loop
        //

        const int id = current_id_++;
        assert(edges_.find(id) == edges_.end());
        // assert(nodes_.find(from) != nodes_.end());
        // assert(nodes_.find(to) != nodes_.end());
        edges_.insert(std::make_pair(id, Edge(id, from_idx, to_idx)));

        // update neighbor list
        auto& [src, from] = edge_output(from_idx);
        auto& [dst, to] = edge_input(to_idx);

        // assert(nodes_.find(from) != nodes_.end());
        src.neighbors.push_back(dst.id);

        return id;
    }

    void erase_edge(const int edge_id)
    {
        // This is a bit lazy, we find the pointer here, but we refind it when we erase the edge
        // based on id key.
        assert(edges_.find(edge_id) != edges_.end());
        const Edge& edge = edges_.find(edge_id)->second;

        // update neighbor list
        {
            assert(nodes_.find(edge.from) != nodes_.end());
            auto& node = nodes_.find(edge.from)->second;
            node.erase_edge(edge.to);
        }

        edges_.erase(edge_id);
    }

    Edge& edge_from_to(const InputAttribute& to)
    {
        for (auto& [id, edge] : edges_)
        {
            if (edge.to == to.id)
            {
                return edge;
            }
        }

        throw std::runtime_error("not found");
    }
};

} // namespace example
