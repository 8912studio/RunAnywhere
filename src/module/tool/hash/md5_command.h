#pragma once

#include "module/tool/hash/hash_command.h"

namespace ra::mod::tool::hash {

class MD5Command : public HashCommand {
public:
    static std::wstring Keyword();

public:
    std::wstring GetKeyword() override;
    std::shared_ptr<help::markdown::element::Element> GetHelpContent() override;

private:
    HashAlgorithmCreator GetHashAlgorithmCreator();
};

}