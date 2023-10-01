#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"

namespace ra::help {

class BuiltInHelpContentManager : zaf::NonCopyableNonMovable {
public:
    static BuiltInHelpContentManager& Instance();

public:
    std::shared_ptr<markdown::element::Element> GetDescription(const std::wstring& command);
    std::shared_ptr<markdown::element::Element> GetDetail(const std::wstring& command);

private:
    class HelpContent {
    public:
        std::shared_ptr<markdown::element::Element> description;
        std::shared_ptr<markdown::element::Element> detail;
    };

private:
    static std::shared_ptr<markdown::element::Element> LoadBuiltInHelpContent(
        const std::wstring& command);

    static std::shared_ptr<markdown::element::Element> LoadMarkdownFromResource(
        const std::wstring& command);

    static markdown::element::ElementList RemoveContentBeforeHeader(
        const markdown::element::ElementList& elements);

    static std::shared_ptr<markdown::element::Element> GetDescriptionFromDetail(
        const markdown::element::Element& root);

private:
    BuiltInHelpContentManager() = default;

    const HelpContent& GetHelpContent(const std::wstring& command);

private:
    std::map<std::wstring, HelpContent> map_;
};

}