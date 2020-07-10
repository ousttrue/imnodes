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
template<typename NodeType>
class Graph
{
    int current_id_ = 0;
    // These contains map to the node id
    std::unordered_map<int, NodeType> nodes_;
    std::unordered_map<int, std::vector<int>> node_neighbors_;

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

    NodeType& node(const int id)
    {
        return const_cast<NodeType&>(static_cast<const Graph*>(this)->node(id));
    }
    const NodeType& node(const int id) const
    {
        const auto iter = nodes_.find(id);
        assert(iter != nodes_.end());
        return iter->second;
    }

    std::span<const int> neighbors(int node_id) const
    {
        const auto iter = node_neighbors_.find(node_id);
        assert(iter != node_neighbors_.end());
        return iter->second;
    }

    // Capacity

    size_t num_edges_from_node(const int id) const
    {
        auto iter = edges_from_node_.find(id);
        assert(iter != edges_from_node_.end());
        return iter->second;
    }

    // Modifiers

    int insert_node(const NodeType& node)
    {
        const int id = current_id_++;
        assert(!nodes_.contains(id));
        nodes_.insert(std::make_pair(id, node));
        edges_from_node_.insert(std::make_pair(id, 0));
        node_neighbors_.insert(std::make_pair(id, std::vector<int>()));
        return id;
    }

    void erase_node(const int id)
    {

        // first, remove any potential dangling edges
        {
            static std::vector<int> edges_to_erase;

            for (auto &[_, edge] : edges_)
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
        edges_from_node_.erase(id);
        node_neighbors_.erase(id);
    }

    int insert_edge(const int from, const int to)
    {
        const int id = current_id_++;
        assert(edges_.find(id) == edges_.end());
        assert(nodes_.find(from) != nodes_.end());
        assert(nodes_.find(to) != nodes_.end());
        edges_.insert(std::make_pair(id, Edge(id, from, to)));

        // update neighbor count
        assert(edges_from_node_.find(from) != edges_from_node_.end());
        edges_from_node_.find(from)->second += 1;
        // update neighbor list
        assert(node_neighbors_.find(from) != node_neighbors_.end());
        node_neighbors_.find(from)->second.push_back(to);

        return id;
    }

    void erase_edge(const int edge_id)
    {
        // This is a bit lazy, we find the pointer here, but we refind it when we erase the edge
        // based on id key.
        assert(edges_.find(edge_id) != edges_.end());
        const Edge& edge = edges_.find(edge_id)->second;

        // update neighbor count
        assert(edges_from_node_.find(edge.from) != edges_from_node_.end());
        int& edge_count = edges_from_node_.find(edge.from)->second;
        assert(edge_count > 0);
        edge_count -= 1;

        // update neighbor list
        {
            assert(node_neighbors_.find(edge.from) != node_neighbors_.end());
            std::vector<int>& neighbors = node_neighbors_.find(edge.from)->second;
            auto iter = std::find(neighbors.begin(), neighbors.end(), edge.to);
            assert(iter != neighbors.end());
            neighbors.erase(iter);
        }

        edges_.erase(edge_id);
    }
};

template<typename NodeType, typename Visitor>
void dfs_traverse(const Graph<NodeType>& graph, const int start_node, Visitor visitor)
{
    std::stack<int> stack;

    stack.push(start_node);

    while (!stack.empty())
    {
        const int current_node = stack.top();
        stack.pop();

        visitor(current_node);

        for (const int neighbor : graph.neighbors(current_node))
        {
            stack.push(neighbor);
        }
    }
}
} // namespace example
