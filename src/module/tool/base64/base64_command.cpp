#include "module/tool/base64/base64_command.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/creation.h>

namespace ra::mod::tool::base64 {

CommandBrief Base64Command::Brief() {
    return CommandBrief{ L"b64", {} };
}


Base64Command::Base64Command(const utility::CommandLine&) {
    //Do nothing in constructor. We parse command line in Interpret() method.
}


CommandBrief Base64Command::GetBrief() {
    return Brief();
}


help::content::Content Base64Command::GetHelpContent() {
    return {};
}


bool Base64Command::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    if (is_reusing) {
        return false;
    }

    auto parse_result = Parse(command_line);
    preview_control_ = CreatePreviewControl(parse_result);
    return true;
}


std::shared_ptr<CommandPreviewControl> Base64Command::CreatePreviewControl(
    const Base64CommandParseResult& parse_result) {

    //Try to decode data first.
    std::optional<std::vector<std::byte>> decoded_data;
    bool can_decode{};
    if (!parse_result.operation || *parse_result.operation == Base64Operation::Decode) {
        try {
            decoded_data = zaf::Base64Decode(zaf::ToUTF8String(parse_result.input_text));
            can_decode = true;
        }
        catch (const zaf::Error&) {

        }
    }

    //Determinate which operation should we perform.
    //Use specified operation if there is. Or use decode if the data can be decoded, otherwise use 
    //encode.
    Base64Operation operation =
        parse_result.operation ? *parse_result.operation : 
        can_decode ? Base64Operation::Decode : Base64Operation::Encode; 

    if (operation == Base64Operation::Encode) {
        return CreateEncodePreviewControl(parse_result);
    }
    else if (decoded_data) {
        return CreateDecodePreviewControl(*decoded_data, parse_result);
    }
    else {
        //Fail to decode data.
        return nullptr;
    }
}


std::shared_ptr<CommandPreviewControl> Base64Command::CreateEncodePreviewControl(
    const Base64CommandParseResult& parse_result) {

    std::string encoded_text;
    if (parse_result.encoding == TextEncoding::UTF8) {
        auto utf8_input = zaf::ToUTF8String(parse_result.input_text);
        encoded_text = zaf::Base64Encode(utf8_input.data(), utf8_input.size());
    }
    else {
        const auto& utf16_input = parse_result.input_text;
        encoded_text = zaf::Base64Encode(utf16_input.data(), utf16_input.size() * sizeof(wchar_t));
    }

    auto result = zaf::Create<TextPreviewControl>();
    result->SetText(zaf::FromUTF8String(encoded_text));
    result->SetWrapText(true);
    return result;
}


std::shared_ptr<CommandPreviewControl> Base64Command::CreateDecodePreviewControl(
    const std::vector<std::byte>& decoded_data,
    const Base64CommandParseResult& parse_result) {

    return nullptr;
}


std::shared_ptr<CommandPreviewControl> Base64Command::GetPreviewControl() {
    return preview_control_;
}


void Base64Command::Execute() {

}

}