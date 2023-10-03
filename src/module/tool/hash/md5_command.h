#pragma once

#include "module/tool/hash/hash_command.h"

namespace ra::mod::tool::hash {

class MD5Command : public HashCommand {
public:
    static std::wstring Keyword();

public:
    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;

private:
    HashAlgorithmCreator GetHashAlgorithmCreator();
};

}