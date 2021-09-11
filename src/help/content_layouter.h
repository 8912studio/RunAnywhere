#pragma once

#include "help/content/content.h"
#include "help/content_layout_info.h"

namespace ra::help {

class ContentLayouter {
public:
    ContentLayouter() = default;

    ContentLayouter(const ContentLayouter&) = delete;
    ContentLayouter& operator=(const ContentLayouter&) = delete;

    void SetLayoutWidth(float width);
    void Layout(const content::Content& content);

    float GetTotalHeight() const {
        return current_y_;
    }

    const std::vector<CellVerticalLayoutInfo>& GetLayoutInfos() const {
        return layout_infos_;
    }

private:
    void Relayout();
    std::vector<CellHorizontalLayoutInfo> CreateLineLayoutInfos(const content::Line& line);
    std::vector<CellHorizontalLayoutInfo> CreateTitleLineLayoutInfos(
        const content::TitleLine& title_line);
    std::vector<CellHorizontalLayoutInfo> CreateBodyLineLayoutInfos(
        const content::BodyLine& body_line);
    std::vector<CellHorizontalLayoutInfo> CreateTwoPartsLineLayoutInfos(
        const content::TwoPartsLine& two_parts_line);

private:
    content::Content content_;
    float layout_width_{};

    float current_y_{};
    std::vector<CellVerticalLayoutInfo> layout_infos_;
};

}