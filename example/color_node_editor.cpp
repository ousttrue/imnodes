#include "node_editor.h"
#include "graph.h"
#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>
#include <SDL_keycode.h>
#include <SDL_timer.h>

namespace example
{

static float current_time_seconds = 0.f;

class ColorNodeEditor
{
public:
    Graph<std::shared_ptr<UiNode>, InputPtr, OutputPtr> graph_;

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
            for (auto& [id, node] : graph_.nodes_)
            {
                node.value->evaluate(graph_);
            }
        }
    }

    void drawNodes()
    {
        imnodes::BeginNodeEditor();

        for (auto& [id, node] : graph_.nodes_)
        {
            node.value->show(graph_);
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

            if (ImGui::MenuItem("output"))
            {
                auto ui_node = UiNode::CreateOutput(graph_);
                imnodes::SetNodeScreenSpacePos(ui_node->id(), click_pos);
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
