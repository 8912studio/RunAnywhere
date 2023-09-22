#pragma once

#include <string>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"

namespace ra::help {

class CommandHelpContentFactory {
public:
    explicit CommandHelpContentFactory(std::wstring command);

    //Throws zaf::Error if fail to load help content.
    std::shared_ptr<markdown::element::Element> LoadHelpContent() const;

private:
    std::wstring command_;
};

}