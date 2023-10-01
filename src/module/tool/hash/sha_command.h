#pragma once

#include "module/tool/hash/hash_command.h"
#include "module/tool/hash/sha_algorithm_type.h"

namespace ra::mod::tool::hash {

class SHACommand : public HashCommand {
public:
    static std::wstring Keyword();

public:
    CommandBrief GetBrief() override;
    std::shared_ptr<help::markdown::element::Element> GetHelpContent() override;

private:
    HashCommandParseResult ParseCommand(const utility::CommandLine& command_line) override;
    HashAlgorithmCreator GetHashAlgorithmCreator() override;

private:
    std::optional<SHAAlgorithmType> sha_algorithm_type;
};

}