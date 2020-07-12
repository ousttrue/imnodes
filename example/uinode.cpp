#include "uinode.h"
#include <imnodes.h>
#include <imgui.h>
#include <SDL_timer.h>

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
        const float label_width = ImGui::CalcTextSize(input.data->name().data()).x;
        ImGui::TextUnformatted(input.data->name().data());
        {
            ImGui::SameLine();
            ImGui::PushItemWidth(node_width - label_width);
            ImGui::DragFloat("##hidelabel", &input.data->value(), 0.01f);
            ImGui::PopItemWidth();
        }
        imnodes::EndInputAttribute();
    }

    // outputs
    ImGui::Spacing();
    for (auto& output : node.outputs)
    {
        imnodes::BeginOutputAttribute(output.id);
        const float label_width = ImGui::CalcTextSize(output.data->name().data()).x;
        ImGui::Indent(node_width - label_width);
        ImGui::TextUnformatted(output.data->name().data());
        imnodes::EndOutputAttribute();
    }

    imnodes::EndNode();
}

void evaluate_recursive(GraphType& graph, GraphType::Node& dst, uint32_t frame)
{
    for (auto& to : dst.inputs)
    {
        auto& edge = graph.edge_from_to(to);
        auto& [src, from] = graph.edge_output(edge.from);
    }
}

void UiNode::evaluate(GraphType& graph, GraphType::Node& node, uint32_t frame)
{
    if (node.outputs.empty())
    {
        // starts from end node
        evaluate_recursive(graph, node, frame);

        ImU32 color = IM_COL32(255, 20, 147, 255);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, color);
        ImGui::Begin(node.data->name().data());
        ImGui::End();
        ImGui::PopStyleColor();
    }
}

GraphType::Node& UiNode::CreateAdd(GraphType& graph_)
{
    auto lhs = std::make_shared<Pin>("left");
    auto rhs = std::make_shared<Pin>("right");
    auto result = std::make_shared<Pin>("result");
    auto& node = graph_.insert_node(std::make_shared<UiNode>("add", [lhs, rhs, result]() {
        auto l = lhs->value();
        auto r = rhs->value();
        result->value(l + r);
    }));
    node.add_input(lhs);
    node.add_input(rhs);
    node.add_output(result);
    return node;
}

GraphType::Node& UiNode::CreateMultiply(GraphType& graph_)
{
    auto lhs = std::make_shared<Pin>("left");
    auto rhs = std::make_shared<Pin>("right");
    auto result = std::make_shared<Pin>("result");
    auto& node = graph_.insert_node(std::make_shared<UiNode>("multiply", [lhs, rhs, result]() {
        auto l = lhs->value();
        auto r = rhs->value();
        result->value(l * r);
    }));
    node.add_input(lhs);
    node.add_input(rhs);
    node.add_output(result);
    return node;
}

GraphType::Node& UiNode::CreateOutput(GraphType& graph_)
{
    auto& node = graph_.insert_node(std::make_shared<UiNode>("output", []() {
        // do nothing
    }));
    // input only
    node.add_input(std::make_shared<Pin>("r"));
    node.add_input(std::make_shared<Pin>("g"));
    node.add_input(std::make_shared<Pin>("b"));
    return node;
}

GraphType::Node& UiNode::CreateSine(GraphType& graph_)
{
    auto theta = std::make_shared<Pin>("theta");
    auto sine = std::make_shared<Pin>("sine");
    auto& node = graph_.insert_node(std::make_shared<UiNode>("sine", [theta, sine]() {
        auto value = theta->value();
        sine->value(std::abs(std::sin(value)));
    }));
    node.add_input(theta);
    node.add_output(sine);
    return node;
}

GraphType::Node& UiNode::CreateTime(GraphType& graph_)
{
    auto time = std::make_shared<Pin>("time");
    auto& node = graph_.insert_node(std::make_shared<UiNode>("time", [time]() {
        static float current_time_seconds = 0.f;
        // Update timer context
        current_time_seconds = 0.001f * SDL_GetTicks();
        time->value(current_time_seconds);
    }));
    // output only
    node.add_output(time);
    return node;
}

} // namespace example
