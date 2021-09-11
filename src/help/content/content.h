#pragma once

#include <memory>
#include <string>
#include <vector>
#include "help/content/body_line.h"
#include "help/content/line.h"
#include "help/content/title_line.h"
#include "help/content/two_parts_line.h"

namespace ra::help::content {

class Content {
public:
    void AddTitleLine(const std::wstring& text) {
        lines_.push_back(std::make_shared<TitleLine>(text));
    }

    void AddBodyLine(const std::wstring& text) {
        lines_.push_back(std::make_shared<BodyLine>(text));
    }

    void AddTwoPartsLine(const std::wstring& primary_text, const std::wstring& secondly_text) {
        lines_.push_back(std::make_shared<TwoPartsLine>(primary_text, secondly_text));
    }

    const std::vector<std::shared_ptr<Line>>& Lines() const {
        return lines_;
    }

private:
    std::vector<std::shared_ptr<Line>> lines_;
};

}