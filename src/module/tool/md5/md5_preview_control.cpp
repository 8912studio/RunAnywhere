#include "module/tool/md5/md5_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/graphic/image/image.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include "module/tool/md5/md5_calculating.h"
#include "utility/path_trimming.h"
#include "utility/text_utility.h"

namespace ra::mod::tool::md5 {

ZAF_DEFINE_TYPE(MD5PreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/md5/md5_preview_control.xaml")
ZAF_DEFINE_TYPE_END;


void MD5PreviewControl::SetSourceIcon(const std::wstring& uri) {

	auto icon_image = zaf::Image::FromURI(uri);
	md5SourceIcon->SetBackgroundImage(icon_image);
}


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

	SetSourceIcon(L"res:///resource/file.png");
	SetMD5Encoding(std::nullopt);
	md5SourceControl->SetTextTrimming(utility::CreateTextTrimmingForPath());
	md5SourceControl->SetText(file_path.wstring());

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


void MD5PreviewControl::ShowStringMD5(const std::wstring& string, MD5Encoding encoding) {

	SetSourceIcon(L"res:///resource/string.png");
	SetMD5Encoding(encoding);
	md5SourceControl->SetTextTrimming(zaf::TextTrimmingGranularity::Character);
	md5SourceControl->SetText(utility::ReplaceWhitespacesToVisibleChars(string));

	auto md5 = CalculateStringMD5(string, encoding);
	SetMD5Text(md5);

	ChangeLayout(LayoutType::Result);
}


void MD5PreviewControl::SetMD5Encoding(std::optional<MD5Encoding> encoding) {

	if (!encoding) {
		md5EncodingContainer->SetIsVisible(false);
		return;
	}

	md5EncodingContainer->SetIsVisible(true);

	if (encoding == MD5Encoding::UTF8) {
		md5EncodingControl->SetText(L"UTF8");
	}
	else if (encoding == MD5Encoding::UTF16) {
		md5EncodingControl->SetText(L"UTF16");
	}
	else {
		md5EncodingContainer->SetIsVisible(false);
		return;
	}

	auto container_preferred_size = md5EncodingContainer->CalculatePreferredSize();
	md5EncodingContainer->SetFixedWidth(container_preferred_size.width);
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