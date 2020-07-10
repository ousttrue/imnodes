#pragma once

#include <algorithm>
#include <cassert>
#include <iterator>
#include <stack>
#include <unordered_map>
#include <stddef.h>
#include <utility>
#include <vector>
#include <span>

namespace example
{

// a very simple directional graph
template<typename NodePayload>
class Graph
{
    int current_id_ = 0;

public:
    // These contains map to the node id
    struct Node
    {
        NodePayload payload;
        Node(const NodePayload &payload)
        : payload(payload)
        {           
        }
        
        std::vector<int> neighbors;

        void erase_edge(int to_node)
        {
            auto iter = std::find(neighbors.begin(), neighbors.end(), to_node);
            assert(iter != neighbors.end());
            neighbors.erase(iter);
        }
    };

private:
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
        return const_cast<Node&>(static_cast<const Graph*>(this)->node(id));
    }

    const Node& node(const int id) const
    {
        const auto iter = nodes_.find(id);
        assert(iter != nodes_.end());
        return iter->second;
    }

    // Modifiers

    int insert_node(const NodePayload& node)
    {
        const int id = current_id_++;
        assert(!nodes_.contains(id));
        nodes_.insert(std::make_pair(id, Node(node)));
        return id;
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

    int insert_edge(const int from, const int to)
    {
        const int id = current_id_++;
        assert(edges_.find(id) == edges_.end());
        assert(nodes_.find(from) != nodes_.end());
        assert(nodes_.find(to) != nodes_.end());
        edges_.insert(std::make_pair(id, Edge(id, from, to)));

        // update neighbor list
        assert(nodes_.find(from) != nodes_.end());
        nodes_.find(from)->second.neighbors.push_back(to);

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

template<typename NodePayload, typename Visitor>
void dfs_traverse(const Graph<NodePayload>& graph, const int start_node, Visitor visitor)
{
    std::stack<int> stack;

    stack.push(start_node);

    while (!stack.empty())
    {
        const int current_node = stack.top();
        stack.pop();

        visitor(current_node);

        auto& node = graph.node(current_node);
        for (auto neighbor : node.neighbors)
        {
            stack.push(neighbor);
        }
    }
}
} // namespace example
