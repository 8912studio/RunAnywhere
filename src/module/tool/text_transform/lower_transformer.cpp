#include "module/tool/text_transform/lower_transformer.h"
#include <zaf/base/string/case_conversion.h>

namespace ra::module::tool::text_transform {

CommandBrief LowerTransformer::Brief() {
    return CommandBrief{ L"low", L"Transform text to lowercase" };
}


CommandBrief LowerTransformer::GetBrief() {
    return Brief();
}


help::content::Content LowerTransformer::GetHelpContent() {

	help::content::Content result;

	result.AddTitleLine(L"low command");
	result.AddBodyLine(L"Transform all characters in specified text to lowercase.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"low <text>");
	result.AddBodyLine(
		L"If there are space characters in `text`, enclose it in double quotation marks( \" ).");
	result.AddBodyLine(L"Or use text block(press ALT+T to insert) to contain arbitrary text.");

	return result;
}


void LowerTransformer::Transform(std::wstring& text) {
    zaf::Lowercase(text);
}

}