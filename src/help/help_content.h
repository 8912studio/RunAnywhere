#pragma once

#include <memory>
#include <zaf/base/error/check.h>
#include "utility/markdown/element/element.h"

namespace ra::help {

class HelpContent {
public:
    HelpContent(std::wstring id, std::shared_ptr<utility::markdown::element::Element> element) :
        id_(std::move(id)),
        element_(std::move(element)) {

        ZAF_EXPECT(element_);
    }

    const std::wstring& ID() const {
        return id_;
    }

    const std::shared_ptr<utility::markdown::element::Element>& Element() const {
        return element_;
    }

private:
    std::wstring id_;
    std::shared_ptr<utility::markdown::element::Element> element_;
};

}