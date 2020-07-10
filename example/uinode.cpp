#include "uinode.h"
#include "node.h"
#include <imnodes.h>
#include <imgui.h>

namespace example
{

enum class UiNodeType
{
    add,
    multiply,
    output,
    sine,
    time,
};

struct AddNode : public UiNode
{
    int lhs, rhs;

    void show(Graph<Node>& graph_) const override
    {
        const float node_width = 100.f;
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("add");
        imnodes::EndNodeTitleBar();
        {
            imnodes::BeginInputAttribute(lhs);
            const float label_width = ImGui::CalcTextSize("left").x;
            ImGui::TextUnformatted("left");
            if (graph_.num_edges_from_node(lhs) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(lhs).value, 0.01f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        {
            imnodes::BeginInputAttribute(rhs);
            const float label_width = ImGui::CalcTextSize("right").x;
            ImGui::TextUnformatted("right");
            if (graph_.num_edges_from_node(rhs) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(rhs).value, 0.01f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            imnodes::BeginOutputAttribute(id);
            const float label_width = ImGui::CalcTextSize("result").x;
            ImGui::Indent(node_width - label_width);
            ImGui::TextUnformatted("result");
            imnodes::EndOutputAttribute();
        }

        imnodes::EndNode();
    }

    bool erase(Graph<Node>& graph_) const override
    {
        graph_.erase_node(id);
        graph_.erase_node(lhs);
        graph_.erase_node(rhs);
        return false;
    }
};

struct MultiplyNode : public UiNode
{
    int lhs, rhs;

    void show(Graph<Node>& graph_) const override
    {
        const float node_width = 100.0f;
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("multiply");
        imnodes::EndNodeTitleBar();

        {
            imnodes::BeginInputAttribute(lhs);
            const float label_width = ImGui::CalcTextSize("left").x;
            ImGui::TextUnformatted("left");
            if (graph_.num_edges_from_node(lhs) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(lhs).value, 0.01f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        {
            imnodes::BeginInputAttribute(rhs);
            const float label_width = ImGui::CalcTextSize("right").x;
            ImGui::TextUnformatted("right");
            if (graph_.num_edges_from_node(rhs) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(rhs).value, 0.01f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            imnodes::BeginOutputAttribute(id);
            const float label_width = ImGui::CalcTextSize("result").x;
            ImGui::Indent(node_width - label_width);
            ImGui::TextUnformatted("result");
            imnodes::EndOutputAttribute();
        }

        imnodes::EndNode();
    }

    bool erase(Graph<Node>& graph_) const override
    {
        graph_.erase_node(id);
        graph_.erase_node(lhs);
        graph_.erase_node(rhs);
        return false;
    }
};

struct OutputNode : public UiNode
{
    int r, g, b;

    void show(Graph<Node>& graph_) const override
    {
        const float node_width = 100.0f;
        imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(11, 109, 191, 255));
        imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarHovered, IM_COL32(45, 126, 194, 255));
        imnodes::PushColorStyle(imnodes::ColorStyle_TitleBarSelected, IM_COL32(81, 148, 204, 255));
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("output");
        imnodes::EndNodeTitleBar();

        ImGui::Dummy(ImVec2(node_width, 0.f));
        {
            imnodes::BeginInputAttribute(r);
            const float label_width = ImGui::CalcTextSize("r").x;
            ImGui::TextUnformatted("r");
            if (graph_.num_edges_from_node(r) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(r).value, 0.01f, 0.f, 1.0f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            imnodes::BeginInputAttribute(g);
            const float label_width = ImGui::CalcTextSize("g").x;
            ImGui::TextUnformatted("g");
            if (graph_.num_edges_from_node(g) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(g).value, 0.01f, 0.f, 1.f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            imnodes::BeginInputAttribute(b);
            const float label_width = ImGui::CalcTextSize("b").x;
            ImGui::TextUnformatted("b");
            if (graph_.num_edges_from_node(b) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(b).value, 0.01f, 0.f, 1.0f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }
        imnodes::EndNode();
        imnodes::PopColorStyle();
        imnodes::PopColorStyle();
        imnodes::PopColorStyle();
    }

    bool erase(Graph<Node>& graph_) const override
    {
        graph_.erase_node(id);
        graph_.erase_node(r);
        graph_.erase_node(g);
        graph_.erase_node(b);
        return true;
    }
};

struct SineNode : public UiNode
{
    int input;
    void show(Graph<Node>& graph_) const override
    {
        const float node_width = 100.0f;
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("sine");
        imnodes::EndNodeTitleBar();

        {
            imnodes::BeginInputAttribute(input);
            const float label_width = ImGui::CalcTextSize("number").x;
            ImGui::TextUnformatted("number");
            if (graph_.num_edges_from_node(input) == 0ull)
            {
                ImGui::SameLine();
                ImGui::PushItemWidth(node_width - label_width);
                ImGui::DragFloat("##hidelabel", &graph_.node(input).value, 0.01f, 0.f, 1.0f);
                ImGui::PopItemWidth();
            }
            imnodes::EndInputAttribute();
        }

        ImGui::Spacing();

        {
            imnodes::BeginOutputAttribute(id);
            const float label_width = ImGui::CalcTextSize("output").x;
            ImGui::Indent(node_width - label_width);
            ImGui::TextUnformatted("output");
            imnodes::EndInputAttribute();
        }

        imnodes::EndNode();
    }

    bool erase(Graph<Node>& graph_) const override
    {
        graph_.erase_node(id);
        graph_.erase_node(input);
        return false;
    }
};

struct TimeNode : public UiNode
{
    void show(Graph<Node>& graph_) const override
    {
        imnodes::BeginNode(id);

        imnodes::BeginNodeTitleBar();
        ImGui::TextUnformatted("time");
        imnodes::EndNodeTitleBar();

        imnodes::BeginOutputAttribute(id);
        ImGui::Text("output");
        imnodes::EndOutputAttribute();

        imnodes::EndNode();
    }

    bool erase(Graph<Node>& graph_) const override
    {
        graph_.erase_node(id);
        return false;
    }
};

std::shared_ptr<UiNode> UiNode::CreateAdd(Graph<Node>& graph_)
{
    const Node value(NodeType::value, 0.f);
    const Node op(NodeType::add);

    auto ui_node = std::make_shared<AddNode>();
    // ui_node.type = UiNodeType::add;
    ui_node->lhs = graph_.insert_node(value);
    ui_node->rhs = graph_.insert_node(value);
    ui_node->id = graph_.insert_node(op);

    graph_.insert_edge(ui_node->id, ui_node->lhs);
    graph_.insert_edge(ui_node->id, ui_node->rhs);

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateMultiply(Graph<Node>& graph_)
{
    const Node value(NodeType::value, 0.f);
    const Node op(NodeType::multiply);

    auto ui_node = std::make_shared<MultiplyNode>();
    // ui_node.type = UiNodeType::multiply;
    ui_node->lhs = graph_.insert_node(value);
    ui_node->rhs = graph_.insert_node(value);
    ui_node->id = graph_.insert_node(op);

    graph_.insert_edge(ui_node->id, ui_node->lhs);
    graph_.insert_edge(ui_node->id, ui_node->rhs);

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateOutput(Graph<Node>& graph_)
{
    const Node value(NodeType::value, 0.f);
    const Node out(NodeType::output);

    auto ui_node = std::make_shared<OutputNode>();
    // ui_node.type = UiNodeType::output;
    ui_node->r = graph_.insert_node(value);
    ui_node->g = graph_.insert_node(value);
    ui_node->b = graph_.insert_node(value);
    ui_node->id = graph_.insert_node(out);

    graph_.insert_edge(ui_node->id, ui_node->r);
    graph_.insert_edge(ui_node->id, ui_node->g);
    graph_.insert_edge(ui_node->id, ui_node->b);

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateSine(Graph<Node>& graph_)
{
    const Node value(NodeType::value, 0.f);
    const Node op(NodeType::sine);

    auto ui_node = std::make_shared<SineNode>();
    // ui_node.type = UiNodeType::sine;
    ui_node->input = graph_.insert_node(value);
    ui_node->id = graph_.insert_node(op);

    graph_.insert_edge(ui_node->id, ui_node->input);

    return ui_node;
}

std::shared_ptr<UiNode> UiNode::CreateTime(Graph<Node>& graph_)
{
    auto ui_node = std::make_shared<TimeNode>();

    // ui_node.type = UiNodeType::time;
    ui_node->id = graph_.insert_node(Node(NodeType::time));

    return ui_node;
}

} // namespace example
