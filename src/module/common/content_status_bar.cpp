#include "module/common/content_status_bar.h"
#include "utility/path_trimming.h"
#include "utility/text_utility.h"

namespace ra::mod {

ZAF_OBJECT_IMPL(ContentStatusBar);

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
	SetText(text);
}


void ContentStatusBar::ShowBinary(const std::wstring& binary_text) {

	auto update_guard = BeginUpdate();

	SetTypeIcon(L"res:///resource/binary.png");
	SetEncoding(std::nullopt);
	SetText(binary_text);
}


void ContentStatusBar::ShowNone(const std::wstring& text) {

	auto update_guard = BeginUpdate();

	SetTypeIcon({});
	SetEncoding(std::nullopt);
	SetText(text);
}


void ContentStatusBar::SetTypeIcon(const std::wstring& uri) {

	if (!uri.empty()) {
		auto icon_image = zaf::Image::FromURI(uri);
		typeIcon->SetBackgroundImage(icon_image);
		typeIcon->SetIsVisible(true);
	}
	else {
		typeIcon->SetIsVisible(false);
	}
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


void ContentStatusBar::SetText(const std::wstring& text) {

	contentLabel->SetTextTrimming(zaf::dwrite::TextTrimmingGranularity::Character);
	contentLabel->SetText(utility::ReplaceWhitespacesToVisibleChars(text));
}


void ContentStatusBar::SetIconTooltip(const std::wstring& tooltip) {
	typeIcon->SetTooltip(tooltip);
}


void ContentStatusBar::SetEncodingTooltip(const std::wstring& tooltip) {
	encodingLabel->SetTooltip(tooltip);
}

}