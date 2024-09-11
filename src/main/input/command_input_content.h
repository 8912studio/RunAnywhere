#pragma once

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <zaf/control/text_box.h>
#include <zaf/control/textual/inline_object.h>

namespace ra::main::input {

class CommandInputContent {
public:
    struct ObjectInfo {
        std::size_t index{};
        std::shared_ptr<zaf::textual::InlineObject> object;
    };

    using Visitor = std::function<
        void(const std::variant<std::wstring_view, ObjectInfo>&)
    >;

    static CommandInputContent FromTextBox(const zaf::TextBox& text_box);

public:
    void Visit(const Visitor& visitor) const;

private:
    CommandInputContent(std::wstring text, std::vector<ObjectInfo> objects);

private:
    std::wstring text_;
    std::vector<ObjectInfo> inline_objects_;
};

}
