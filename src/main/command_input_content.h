#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <zaf/control/rich_edit.h>
#include <zaf/control/rich_edit/object_info.h>

namespace ra {

class CommandInputContent {
public:
    using Visitor = std::function<
        void(const std::variant<std::wstring_view, zaf::rich_edit::ObjectInfo>&)
    >;

    static CommandInputContent FromRichEdit(const zaf::RichEdit& rich_edit);

public:
    void Visit(const Visitor& visitor) const;

private:
    CommandInputContent(std::wstring text, std::vector<zaf::rich_edit::ObjectInfo> objects);

private:
    std::wstring text_;
    std::vector<zaf::rich_edit::ObjectInfo> objects_;
};

}
