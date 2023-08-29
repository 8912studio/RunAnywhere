#pragma once

#include <filesystem>
#include <optional>
#include <zaf/control/control_binder.h>
#include <zaf/control/image_box.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include <zaf/control/rich_edit.h>
#include <zaf/base/none.h>
#include <zaf/rx/subject.h>
#include "module/command_preview_control.h"
#include "module/common/content_status_bar.h"
#include "module/common/error_view.h"
#include "module/common/general_input.h"
#include "module/common/text_encoding.h"
#include "module/common/text_content_control.h"
#include "module/tool/hash/hash_algorithm_creator.h"
#include "utility/preview_text_box.h"
#include "utility/progress_circle.h"

namespace ra::mod::tool::hash {

class HashPreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_TYPE;

	explicit HashPreviewControl(HashAlgorithmCreator hash_creator);

	void ShowHash(const GeneralInput& input);

	std::wstring GetText();

	void SetUseUppercase(bool value) {
		use_upper_case_ = value;
	}

	zaf::Frame GetExpectedMargin() override;

	//For unittest.
	zaf::Observable<zaf::None> CalculateFinishedEvent() const {
		return calculate_finished_event_.GetObservable();
	}

protected:
	void AfterParse() override;
	void OnStyleChanged() override;

private:
	enum class LayoutType {
		Progress,
		Error,
		Result,
	};

private:
	void ShowFileHash(const std::filesystem::path& file_path);
	void ShowStringHash(const std::wstring& string, TextEncoding encoding);
	void SetHashText(const std::wstring& hash);
	void AdjustControlStyles();
	void ChangeLayout(LayoutType type);

private:
	ZAF_BIND_CONTROL(ContentStatusBar, contentStatusBar);
	ZAF_BIND_CONTROL(utility::ProgressCircle, progressCircle);
	ZAF_BIND_CONTROL(ErrorView, errorView);
	ZAF_BIND_CONTROL(TextContentControl, hashResultControl);

	HashAlgorithmCreator hash_algorithm_creator_;
	bool use_upper_case_{};

	zaf::Subject<zaf::None> calculate_finished_event_;
};

}