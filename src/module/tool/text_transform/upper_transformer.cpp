#include "module/tool/text_transform/upper_transformer.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::module::tool::text_transform {

CommandBrief UpperTransformer::Brief() {
    return CommandBrief{ L"up", L"Transform text to uppercase" };
}


CommandBrief UpperTransformer::GetBrief() {
    return Brief();
}


help::content::Content UpperTransformer::GetHelpContent() {

	help::content::Content result;

	result.AddTitleLine(L"up command");
	result.AddBodyLine(L"Transform all characters in specified text to uppercase.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"up <text>");
	result.AddBodyLine(
		L"If there are space characters in `text`, enclose it in double quotation marks( \" ).");

	return result;
}


void UpperTransformer::Transform(std::wstring & text) {
    zaf::Uppercase(text);
}

}