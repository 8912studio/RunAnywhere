#include "module/common/content_status_bar.h"
#include <zaf/object/type_definition.h>
#include "utility/path_trimming.h"
#include "utility/text_utility.h"

namespace ra::mod {

ZAF_DEFINE_TYPE(ContentStatusBar)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/content_status_bar.xaml")
ZAF_DEFINE_TYPE_END;

void ContentStatusBar::ShowFile(const std::filesystem::path& file_path) {

	auto update_guard = BeginUpdate();

	SetTypeIcon(L"res:///resource/file.png");
	SetEncoding(std::nullopt);

	contentLabel->SetTextTrimming(utility::CreateTextTrimmingForPath());
	contentLabel->SetText(file_path.wstring());
}


void ContentStatusBar::ShowText(const std::wstring& text, TextEncoding encoding) {

	auto update_guard = BeginUpdate();

	SetTypeIcon(L"res:///resource/string.png");
	SetEncoding(encoding);

	contentLabel->SetTextTrimming(zaf::TextTrimmingGranularity::Character);
	contentLabel->SetText(utility::ReplaceWhitespacesToVisibleChars(text));
}


void ContentStatusBar::SetTypeIcon(const std::wstring& uri) {

	auto icon_image = zaf::Image::FromURI(uri);
	typeIcon->SetBackgroundImage(icon_image);
}


void ContentStatusBar::SetEncoding(std::optional<TextEncoding> encoding) {

	if (!encoding) {
		encodingContainer->SetIsVisible(false);
		return;
	}

	encodingContainer->SetIsVisible(true);

	if (encoding == TextEncoding::UTF8) {
		encodingLabel->SetText(L"UTF8");
	}
	else if (encoding == TextEncoding::UTF16) {
		encodingLabel->SetText(L"UTF16");
	}
	else {
		encodingContainer->SetIsVisible(false);
		return;
	}

	auto container_preferred_size = encodingContainer->CalculatePreferredSize();
	encodingContainer->SetFixedWidth(container_preferred_size.width);
}

}