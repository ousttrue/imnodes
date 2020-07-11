#include "node_editor.h"
#include "graph.h"
#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>

#include <SDL_keycode.h>
#include <SDL_timer.h>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <span>
#include <stack>
#include <memory>

namespace example
{

template<class T>
T clamp(T x, T a, T b)
{
    return std::min(b, std::max(x, a));
}

static float current_time_seconds = 0.f;

// ImU32 evaluate(const Graph<std::shared_ptr<UiNode>>& graph, const int root_node)
// {
//     std::stack<int> postorder;
//     dfs_traverse(
//         graph, root_node, [&postorder](const int node_id) -> void { postorder.push(node_id); });

//     std::stack<float> value_stack;
//     while (!postorder.empty())
//     {
//         const int id = postorder.top();
//         postorder.pop();
//         auto& node = graph.node(id).payload;

//         switch (node.type)
//         {
//         case NodeType::add:
//         {
//             const float rhs = value_stack.top();
//             value_stack.pop();
//             const float lhs = value_stack.top();
//             value_stack.pop();
//             value_stack.push(lhs + rhs);
//         }
//         break;
//         case NodeType::multiply:
//         {
//             const float rhs = value_stack.top();
//             value_stack.pop();
//             const float lhs = value_stack.top();
//             value_stack.pop();
//             value_stack.push(rhs * lhs);
//         }
//         break;
//         case NodeType::sine:
//         {
//             const float x = value_stack.top();
//             value_stack.pop();
//             const float res = std::abs(std::sin(x));
//             value_stack.push(res);
//         }
//         break;
//         case NodeType::time:
//         {
//             value_stack.push(current_time_seconds);
//         }
//         break;
//         case NodeType::value:
//         {
//             // If the edge does not have an edge connecting to another node, then just use the
//             value
//             // at this node. It means the node's input pin has not been connected to anything and
//             // the value comes from the node's UI.
//             if (graph.node(id).neighbors.size() == 0ull)
//             {
//                 value_stack.push(node.value);
//             }
//         }
//         break;
//         default:
//             break;
//         }
//     }

//     // The final output node isn't evaluated in the loop -- instead we just pop
//     // the three values which should be in the stack.
//     assert(value_stack.size() == 3ull);
//     const int b = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
//     value_stack.pop();
//     const int g = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
//     value_stack.pop();
//     const int r = static_cast<int>(255.f * clamp(value_stack.top(), 0.f, 1.f) + 0.5f);
//     value_stack.pop();

//     return IM_COL32(r, g, b, 255);
// }

class ColorNodeEditor
{
public:
    Graph<std::shared_ptr<UiNode>> graph_;
    int root_node_id_ = -1;

    void show()
    {
        // Update timer context
        current_time_seconds = 0.001f * SDL_GetTicks();

        {
            // The node editor window
            ImGui::Begin("color node editor");
            ImGui::TextUnformatted("Edit the color of the output color window using nodes.");
            ImGui::Columns(2);
            ImGui::TextUnformatted("A -- add node");
            ImGui::TextUnformatted("X -- delete selected node or link");
            ImGui::NextColumn();
            ImGui::Checkbox(
                "emulate three button mouse", &imnodes::GetIO().emulate_three_button_mouse.enabled);
            ImGui::Columns(1);

            drawNodes();

            ImGui::End();
        }

        {
            // The color output window
            ImU32 color = IM_COL32(255, 20, 147, 255);
            if (root_node_id_ != -1)
            {
                // color = evaluate(graph_, root_node_id_);
            }
            ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
            ImGui::Begin("output color");
            ImGui::End();
            ImGui::PopStyleColor();
        }
    }

    void drawNodes()
    {
        imnodes::BeginNodeEditor();

        for (auto& [id, node] : graph_.nodes_)
        {
            node.payload->show(graph_);
        }

        for (const auto& [id, edge] : graph_.edges_)
        {
            imnodes::Link(edge.id, edge.from, edge.to);
        }

        const bool open_popup = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) &&
                                imnodes::IsEditorHovered() && ImGui::IsKeyReleased(SDL_SCANCODE_A);

        imnodes::EndNodeEditor();

        {
            // Handle new links
            int start_attr, end_attr;
            if (imnodes::IsLinkCreated(&start_attr, &end_attr))
            {
                // TODO: check link is valid
                graph_.insert_edge(start_attr, end_attr);
            }
        }

        {
            // Handle deleted links
            int link_id;
            if (imnodes::IsLinkDestroyed(&link_id))
            {
                graph_.erase_edge(link_id);
            }
        }

        if (ImGui::IsKeyReleased(SDL_SCANCODE_X))
        {
            {
                // remove selected links
                const int num_selected = imnodes::NumSelectedLinks();
                if (num_selected > 0)
                {
                    static std::vector<int> selected_links;
                    selected_links.resize(static_cast<size_t>(num_selected));
                    imnodes::GetSelectedLinks(selected_links.data());
                    for (const int edge_id : selected_links)
                    {
                        graph_.erase_edge(edge_id);
                    }
                }
            }
            {
                // remove selected nodes
                const int num_selected = imnodes::NumSelectedNodes();
                if (num_selected > 0)
                {
                    static std::vector<int> selected_nodes;
                    selected_nodes.resize(static_cast<size_t>(num_selected));
                    imnodes::GetSelectedNodes(selected_nodes.data());
                    for (const int node_id : selected_nodes)
                    {
                        auto& node = graph_.node(node_id);
                        if (node.payload->name() == "output")
                        {
                            root_node_id_ = -1;
                        }

                        // Erase any additional internal nodes
                        graph_.erase_node(node_id);
                    }
                }
            }
        }

        popup(open_popup);
    }

    void popup(bool open_popup)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.f, 8.f));
        if (!ImGui::IsAnyItemHovered() && open_popup)
        {
            ImGui::OpenPopup("add node");
        }

        if (ImGui::BeginPopup("add node"))
        {
            const ImVec2 click_pos = ImGui::GetMousePosOnOpeningCurrentPopup();

            if (ImGui::MenuItem("add"))
            {
                auto ui_node = UiNode::CreateAdd(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
            }

            if (ImGui::MenuItem("multiply"))
            {
                auto ui_node = UiNode::CreateMultiply(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
            }

            if (ImGui::MenuItem("output") && root_node_id_ == -1)
            {
                auto ui_node = UiNode::CreateOutput(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
                root_node_id_ = ui_node->id();
            }

            if (ImGui::MenuItem("sine"))
            {
                auto ui_node = UiNode::CreateSine(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
            }

            if (ImGui::MenuItem("time"))
            {
                auto ui_node = UiNode::CreateTime(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
            }

            ImGui::EndPopup();
        }
        ImGui::PopStyleVar();
    }
};

static ColorNodeEditor color_editor;

void NodeEditorInitialize()
{
    imnodes::IO& io = imnodes::GetIO();
    io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;
}

void NodeEditorShow() { color_editor.show(); }

void NodeEditorShutdown() {}
} // namespace example
