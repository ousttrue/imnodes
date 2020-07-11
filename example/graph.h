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
template<typename NodeValue, typename InputValue, typename OutputValue>
class Graph
{
    static inline int current_id_ = 0;

public:
    struct InputAttribute
    {
        int id;
        InputValue value;
        InputAttribute(int id, const InputValue& value) : id(id), value(value) {}
    };
    struct OutputAttribute
    {
        int id;
        OutputValue value;
        OutputAttribute(int id, const OutputValue& value) : id(id), value(value) {}
    };

    struct Node
    {
        int id;
        NodeValue value;
        Node(int id, const NodeValue& value) : id(id), value(value) {}

        std::vector<int> neighbors;
        std::vector<InputAttribute> inputs;
        std::vector<OutputAttribute> outputs;

        void erase_edge(int to_node)
        {
            auto iter = std::find(neighbors.begin(), neighbors.end(), to_node);
            assert(iter != neighbors.end());
            neighbors.erase(iter);
        }

        void add_input(const InputValue &input)
        {
            const int id = current_id_++;
            inputs.push_back(InputAttribute(id, input));
        }

        void add_output(const OutputValue &output)
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

    Node& node(int id) { return const_cast<Node&>(static_cast<const Graph*>(this)->node(id)); }

    const Node& node(int id) const
    {
        const auto iter = nodes_.find(id);
        assert(iter != nodes_.end());
        return iter->second;
    }

    const Node& node_from_output(int id) const
    {
        for (auto& [k, v] : nodes_)
        {
            for (auto& output : v.outputs)
            {
                if (output.id == id)
                {
                    return v;
                }
            }
        }

        throw std::runtime_error("not found");
    }

    const Node& node_from_input(int id) const
    {
        for (auto& [k, v] : nodes_)
        {
            for (auto& input : v.inputs)
            {
                if (input.id == id)
                {
                    return v;
                }
            }
        }

        throw std::runtime_error("not found");
    }

    // Modifiers

    NodeValue insert_node(const std::function<NodeValue(int)>& create)
    {
        const int id = current_id_++;
        assert(!nodes_.contains(id));
        auto payload = create(id);
        nodes_.insert(std::make_pair(id, Node(id, payload)));
        return payload;
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
    int insert_edge(const int from, const int to)
    {
        //
        // TODO: avoid loop
        //

        const int id = current_id_++;
        assert(edges_.find(id) == edges_.end());
        // assert(nodes_.find(from) != nodes_.end());
        // assert(nodes_.find(to) != nodes_.end());
        edges_.insert(std::make_pair(id, Edge(id, from, to)));

        // update neighbor list
        auto from_node = node_from_output(from);
        auto to_node = node_from_input(to);

        // assert(nodes_.find(from) != nodes_.end());
        from_node.neighbors.push_back(to_node.id);

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
};

} // namespace example
