#include "module/tool/rgb/rgb_command.h"
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/split.h>
#include <zaf/base/string/to_numeric.h>
#include <zaf/creation.h>
#include "module/calculator/parse/decimal_number_parser.h"
#include "module/calculator/parse/non_decimal_number_parser.h"
#include "utility/clipboard.h"

namespace ra::module::tool::rgb {
namespace {

constexpr std::size_t RGBAComponentCount = 4;

class RGBCommandParseContext {
public:
    zaf::Color intermediate_color;
    float additional_alpha{ 1.f };
    RGBCommandParseResult result;
};


void ParseSwitch(const std::wstring& argument, RGBCommandParseResult& result) {

    if (argument == L"/d") {
        result.format = RGBCommandParseResult::Format::DecimalIntegerComponents;
    }
    else if (argument == L"/f") {
        result.format = RGBCommandParseResult::Format::DecimalFloatComponents;
    }
    else if (argument == L"/x") {
        result.format = RGBCommandParseResult::Format::HexComponents;
    }
    else if (argument == L"/a") {
        result.reserve_alpha = true;
    }
}


bool ParseColorFromARGBFormat(const std::wstring& argument, zaf::Color& color) {

    constexpr std::size_t long_notation_length = 9;
    constexpr std::size_t short_notation_length = 7;

    std::wstring revised_notation(1, L'#');
    if (argument.length() < short_notation_length) {

        revised_notation += std::wstring(short_notation_length - argument.length(), L'0');
    }
    else if (short_notation_length < argument.length() &&
        argument.length() < long_notation_length) {

        revised_notation += std::wstring(long_notation_length - argument.length(), L'0');
    }

    revised_notation += argument.substr(1);

    try {

        zaf::Color::Type->GetParser()->ParseFromAttribute(revised_notation, color);
        return true;
    }
    catch (const zaf::Error&) {
        return false;
    }
}


std::optional<float> ConvertValueFromComponentNode(
    const calculator::OperandNode& node,
    int base,
    bool use_float_format) {

    if (node.base != base) {
        return std::nullopt;
    }

    if (use_float_format) {

        float value{};
        if (!zaf::TryToNumeric(node.text, value)) {
            return std::nullopt;
        }

        if (value < 0 || value > 1.f) {
            return std::nullopt;
        }

        return value;
    }
    else {

        int value{};
        if (!zaf::TryToNumeric(node.text, value, zaf::ToNumericOptions().Base(base))) {
            return std::nullopt;
        }

        if (value < 0 || value > 255) {
            return std::nullopt;
        }

        return float(value) / 255.f;
    }
}


std::optional<zaf::Color> ConvertColorFromComponentNodes(
    const std::shared_ptr<calculator::OperandNode> component_nodes[RGBAComponentCount]) {

    const auto& first_node = component_nodes[0];
    if (first_node->base != 10 &&
        first_node->base != 16) {
        return std::nullopt;
    }

    bool use_float_format = zaf::Contain(first_node->text, L'.');

    float rgba[RGBAComponentCount]{};
    rgba[3] = 1.f; //Alpha is default to 1.0

    for (auto index : zaf::Range(0u, RGBAComponentCount)) {

        const auto& node = component_nodes[index];
        if (!node) {
            continue;
        }

        auto value = ConvertValueFromComponentNode(
            *node,
            static_cast<int>(first_node->base),
            use_float_format);

        if (!value) {
            return std::nullopt;
        }

        rgba[index] = *value;
    }

    return zaf::Color{ rgba[0], rgba[1], rgba[2], rgba[3] };
}


std::shared_ptr<calculator::OperandNode> ParseComponentWithParser(
    const std::wstring& text,
    calculator::Parser* parser) {

    calculator::ParseContext context(text);
    calculator::ParseResult result;
    auto status = parser->Parse(context, result);
    if (status != calculator::ParseStatus::Ok) {
        return nullptr;
    }

    if (context.GetCurrentIndex() != context.GetLength()) {
        return nullptr;
    }

    return std::dynamic_pointer_cast<calculator::OperandNode>(result.GetExpressionRootNode());
}


std::shared_ptr<calculator::OperandNode> ParseComponent(
    const std::wstring& text,
    std::optional<int> force_base) {

    if (!force_base || *force_base == 10) {

        auto result = ParseComponentWithParser(text, calculator::DecimalNumberParser::Instance());
        if (result) {
            return result;
        }
    }

    if (!force_base || *force_base == 16) {

        //Revise prefix in order to parse it successfully.
        auto hex_text = zaf::ToLowercased(text);
        if (hex_text == L"0" || hex_text == L"0x" || hex_text == L"x") {
            hex_text = L"0x0";
        }

        auto result = ParseComponentWithParser(hex_text, calculator::NonDecimalNumberParser::Hex());
        if (result) {
            return result;
        }
    }

    return nullptr;
}


bool ParseColorFromRGBAComponentFormat(const std::wstring& argument, zaf::Color& color) {

    auto components = zaf::Split(argument, L',');
    if (components.empty() || components.size() > RGBAComponentCount) {
        return false;
    }

    //Remove the last empty part, to access incomplete input while user is typing.
    if (components.back().empty()) {
        components.pop_back();
    }

    std::shared_ptr<calculator::OperandNode> parsed_nodes[RGBAComponentCount]{};

    for (auto index : zaf::Range(0u, components.size())) {

        std::optional<int> force_base;
        if (index != 0) {
            force_base = static_cast<int>(parsed_nodes[0]->base);
        }

        auto node = ParseComponent(components[index], force_base);
        if (!node) {
            return false;
        }

        parsed_nodes[index] = node;
    }

    auto converted_color = ConvertColorFromComponentNodes(parsed_nodes);
    if (!converted_color) {
        return false;
    }

    color = *converted_color;
    return true;
}


bool ParseAdditionalAlpha(const std::wstring& argument, float& additional_alpha) {

    auto value_text = argument.substr(1);
    if (value_text.empty()) {
        additional_alpha = 1.f;
        return true;
    }

    auto operand_node = ParseComponent(value_text, std::nullopt);
    if (!operand_node) {
        return false;
    }

    if (operand_node->base != 10 && operand_node->base != 16) {
        return false;
    }

    bool use_float_format = zaf::Contain(operand_node->text, L'.');
    auto value = ConvertValueFromComponentNode(
        *operand_node,
        static_cast<int>(operand_node->base),
        use_float_format);

    if (!value) {
        return false;
    }

    additional_alpha = *value;
    return true;
}


bool ParseArgument(const std::wstring& argument, RGBCommandParseContext& context) {

    if (argument.empty()) {
        return false;
    }

    auto prefix = argument.front();
    if (prefix == L'/') {
        ParseSwitch(argument, context.result);
        return true;
    }
    else if (prefix == L'$') {
        return ParseAdditionalAlpha(argument, context.additional_alpha);
    }
    else if (prefix == L'#') {
        return ParseColorFromARGBFormat(argument, context.intermediate_color);
    }
    else {
        return ParseColorFromRGBAComponentFormat(argument, context.intermediate_color);
    }
}

}

CommandBrief RGBCommand::GetBrief() {
    return CommandBrief{
        L"rgb",
        L"Show color using RGB format"
    };
}


std::optional<RGBCommandParseResult> RGBCommand::Parse(const utility::CommandLine& command_line) {

    RGBCommandParseContext context;

    for (const auto& each_argument : command_line.Arguments()) {

        if (!ParseArgument(each_argument, context)) {
            return std::nullopt;
        }
    }

    context.result.color = context.intermediate_color;
    context.result.color.a *= context.additional_alpha;
    return context.result;
}


RGBCommand::RGBCommand(const utility::CommandLine& command_line) {

    auto parse_result = Parse(command_line);
    if (parse_result) {
        preview_control_ = zaf::Create<RGBPreviewControl>(*parse_result);
    }
}


help::content::Content RGBCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"RGB command");
    result.AddBodyLine(L"Show RGB color.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(
        L"rgb R,G,B\r\n"
        L"rgb R,G,B,A"
    );
    result.AddBodyLine(
        L"Show color specified with `R`, `G`, `B`, `A` components. Components can be following "
        L"forms:\r\n" 
        "- Integers, range from 0 to 255.\r\n"
        "- Floats, range from 0 to 1.0.\r\n"
        "- Hex, range from 0x0 to 0xFF.\r\n"
        "All components must be the same form.\r\n"
        "If `A` is omitted, the opacity is 100%."
    );
    result.AddBodyLine(
        L"rgb #RRGGBB\r\n"
        L"rgb #AARRGGBB");
    result.AddBodyLine(L"Show color specified with hex RGB or ARGB format.");

    result.AddBodyLine(
        L"rgb <...> $A\r\n"
        L"Specify additional alpha to the color. `A` can be integer, float or hex."
    );

    result.AddTitleLine(L"Switchs");
    result.AddTwoPartsLine(L"/a", L"Reserve opacity.");
    result.AddTwoPartsLine(L"/d", L"Show color components in integer form.");
    result.AddTwoPartsLine(L"/f", L"Show color components in float form.");
    result.AddTwoPartsLine(L"/x", L"Show color components in hex form.");

    result.AddTitleLine(L"Remarks");
    result.AddBodyLine(
        L"If the input color's opacity is not 100%, it will be transformed to another color which "
        "opacity is 100% by default.\r\n"
        "\r\n"
        "For example, an input color #CC000000 would be transformed to #FF333333 and shown in "
        "result.\r\n"
        "\r\n"
        "If you want to reserve its opacity, you can specify `/a` "
        "switch. However, the appearance of color will not changed, because its opacity is "
        "combined with the white background."
    );

    return result;
}


std::shared_ptr<CommandPreviewControl> RGBCommand::GetPreviewControl() {

    return preview_control_;
}


void RGBCommand::Execute() {

    if (preview_control_) {
        utility::SetStringToClipboard(preview_control_->GetText());
    }
}

}