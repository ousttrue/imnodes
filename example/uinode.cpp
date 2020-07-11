#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>

namespace example
{

void UiNode::show(Graph<std::shared_ptr<UiNode>>& graph_) const
{
    const float node_width = 100.f;
    imnodes::BeginNode(id());

    // header
    imnodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name().data());
    imnodes::EndNodeTitleBar();

    // inputs
    auto node = graph_.node(id());
    for (auto& input : node.inputs)
    {
        imnodes::BeginInputAttribute(input.id);
        const float label_width = ImGui::CalcTextSize(input.name.c_str()).x;
        ImGui::TextUnformatted(input.name.c_str());
        // if (graph_.node(lhs).neighbors.size() == 0ull)
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(node_width - label_width);
            ImGui::DragFloat("##hidelabel", &input.value, 0.01f);
            ImGui::PopItemWidth();
        }
        imnodes::EndInputAttribute();
    }

    // outputs
    ImGui::Spacing();
    for (auto& output : node.outputs)
    {
        imnodes::BeginOutputAttribute(output.id);
        const float label_width = ImGui::CalcTextSize(output.name.c_str()).x;
        ImGui::Indent(node_width - label_width);
        ImGui::TextUnformatted(output.name.c_str());
        imnodes::EndOutputAttribute();
    }

    imnodes::EndNode();
}

std::shared_ptr<UiNode> UiNode::CreateAdd(Graph<std::shared_ptr<UiNode>>& graph_)
{
    auto ui_node =
        graph_.insert_node([](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "add")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input("left");
    node.add_input("right");
    node.add_output("result");

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateMultiply(Graph<std::shared_ptr<UiNode>>& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "multiply")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input("left");
    node.add_input("right");
    node.add_output("result");

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateOutput(Graph<std::shared_ptr<UiNode>>& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "output")); });

    auto& node = graph_.node(ui_node->id());
    // input only
    node.add_input("r");
    node.add_input("g");
    node.add_input("b");

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateSine(Graph<std::shared_ptr<UiNode>>& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "sine")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input("theta");
    node.add_output("value");

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateTime(Graph<std::shared_ptr<UiNode>>& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "time")); });

    auto& node = graph_.node(ui_node->id());
    // output only
    node.add_output("time");

    return ui_node;
}

} // namespace example
