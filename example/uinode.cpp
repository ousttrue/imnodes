#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>

namespace example
{

void UiNode::show(GraphType& graph_) const
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
        const float label_width = ImGui::CalcTextSize(input.value->name().data()).x;
        ImGui::TextUnformatted(input.value->name().data());
        // if (graph_.node(lhs).neighbors.size() == 0ull)
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

void UiNode::evaluate(GraphType& graph)
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

std::shared_ptr<UiNode> UiNode::CreateAdd(GraphType& graph_)
{
    auto ui_node =
        graph_.insert_node([](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "add")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input(std::make_shared<Input>("left"));
    node.add_input(std::make_shared<Input>("right"));
    node.add_output(std::make_shared<Output>("result"));

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateMultiply(GraphType& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "multiply")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input(std::make_shared<Input>("left"));
    node.add_input(std::make_shared<Input>("right"));
    node.add_output(std::make_shared<Output>("result"));

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateOutput(GraphType& graph_)
{
    auto ui_node = graph_.insert_node(
        [](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "output")); });

    auto& node = graph_.node(ui_node->id());
    // input only
    node.add_input(std::make_shared<Input>("r"));
    node.add_input(std::make_shared<Input>("g"));
    node.add_input(std::make_shared<Input>("b"));

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateSine(GraphType& graph_)
{
    auto ui_node =
        graph_.insert_node([](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "sine")); });

    auto& node = graph_.node(ui_node->id());
    node.add_input(std::make_shared<Input>("theta"));
    node.add_output(std::make_shared<Output>("value"));

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateTime(GraphType& graph_)
{
    auto ui_node =
        graph_.insert_node([](int id) { return std::shared_ptr<UiNode>(new UiNode(id, "time")); });

    auto& node = graph_.node(ui_node->id());
    // output only
    node.add_output(std::make_shared<Output>("time"));

    return ui_node;
}

} // namespace example
