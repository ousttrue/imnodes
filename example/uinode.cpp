#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>

namespace example
{

void UiNode::show(GraphType& graph, GraphType::Node& node) const
{
    const float node_width = 100.f;
    imnodes::BeginNode(node.id);

    // header
    imnodes::BeginNodeTitleBar();
    ImGui::TextUnformatted(name().data());
    imnodes::EndNodeTitleBar();

    // inputs
    for (auto& input : node.inputs)
    {
        imnodes::BeginInputAttribute(input.id);
        const float label_width = ImGui::CalcTextSize(input.value->name().data()).x;
        ImGui::TextUnformatted(input.value->name().data());
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(node_width - label_width);
            ImGui::DragFloat("##hidelabel", input.value->ptr(), 0.01f);
            ImGui::PopItemWidth();
        }
        imnodes::EndInputAttribute();
    }

    // outputs
    ImGui::Spacing();
    for (auto& output : node.outputs)
    {
        imnodes::BeginOutputAttribute(output.id);
        const float label_width = ImGui::CalcTextSize(output.value->name().data()).x;
        ImGui::Indent(node_width - label_width);
        ImGui::TextUnformatted(output.value->name().data());
        imnodes::EndOutputAttribute();
    }

    imnodes::EndNode();
}

void UiNode::evaluate(GraphType& graph, GraphType::Node& node)
{
    if (name() == "output")
    {
        ImU32 color = IM_COL32(255, 20, 147, 255);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
        ImGui::Begin("output color");
        ImGui::End();
        ImGui::PopStyleColor();
    }
}

GraphType::Node& UiNode::CreateAdd(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::make_shared<UiNode>("add"));
    node.add_input(std::make_shared<Input>("left"));
    node.add_input(std::make_shared<Input>("right"));
    node.add_output(std::make_shared<Output>("result"));
    return node;
}

GraphType::Node& UiNode::CreateMultiply(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::shared_ptr<UiNode>(new UiNode("multiply")));
    node.add_input(std::make_shared<Input>("left"));
    node.add_input(std::make_shared<Input>("right"));
    node.add_output(std::make_shared<Output>("result"));
    return node;
}

GraphType::Node& UiNode::CreateOutput(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::shared_ptr<UiNode>(new UiNode("output")));
    // input only
    node.add_input(std::make_shared<Input>("r"));
    node.add_input(std::make_shared<Input>("g"));
    node.add_input(std::make_shared<Input>("b"));
    return node;
}

GraphType::Node& UiNode::CreateSine(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::shared_ptr<UiNode>(new UiNode("sine")));
    node.add_input(std::make_shared<Input>("theta"));
    node.add_output(std::make_shared<Output>("value"));
    return node;
}

GraphType::Node& UiNode::CreateTime(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::shared_ptr<UiNode>(new UiNode("time")));
    // output only
    node.add_output(std::make_shared<Output>("time"));
    return node;
}

} // namespace example
