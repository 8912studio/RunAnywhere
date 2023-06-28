#include "module/tool/md5/md5_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include "module/tool/md5/md5_calculating.h"

namespace ra::mod::tool::md5 {

ZAF_DEFINE_TYPE(MD5PreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/md5/md5_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void MD5PreviewControl::ChangeLayout(LayoutType type) {

	progressCircle->SetIsVisible(false);
	errorControl->SetIsVisible(false);
	md5ResultControl->SetIsVisible(false);

	switch (type) {
	case LayoutType::Progress:
		progressCircle->SetIsVisible(true);
		break;
	case LayoutType::Error:
		errorControl->SetIsVisible(true);
		break;
	case LayoutType::Result:
		md5ResultControl->SetIsVisible(true);
		break;
	default:
		break;
	}
}


void MD5PreviewControl::ShowFileMD5(const std::filesystem::path& file_path) {

	contentStatusBar->ShowFile(file_path);
	contentStatusBar->SetIconTooltip(L"Input is file");

	ChangeLayout(LayoutType::Progress);

	Subscriptions() += CalculateFileMD5(file_path).ObserveOn(zaf::Scheduler::Main()).Subscribe(
		[this](const MD5Result& md5_result) {

		if (md5_result.md5.empty()) {

			progressCircle->SetMaxValue(md5_result.total_size);
			progressCircle->SetValue(md5_result.current_size);
		}
		else {

			SetMD5Text(md5_result.md5);
			ChangeLayout(LayoutType::Result);
		}
	}, 
	[this](const zaf::Error&) {
		ChangeLayout(LayoutType::Error);
	});
}


void MD5PreviewControl::ShowStringMD5(const std::wstring& string, TextEncoding encoding) {

	contentStatusBar->ShowText(string, encoding);
	contentStatusBar->SetIconTooltip(L"Input is text");
	contentStatusBar->SetEncodingTooltip(
		encoding == TextEncoding::UTF8 ? L"Input text is UTF-8" : L"Input text is UTF-16");

	auto md5 = CalculateStringMD5(string, encoding);
	SetMD5Text(md5);

	ChangeLayout(LayoutType::Result);
}


void MD5PreviewControl::SetMD5Text(const std::wstring& md5) {

	if (use_upper_case_) {
		md5ResultControl->SetText(zaf::ToUppercased(md5));
	}
	else {
		md5ResultControl->SetText(md5);
	}
}


std::wstring MD5PreviewControl::GetText() {
	return md5ResultControl->Text();
}

}