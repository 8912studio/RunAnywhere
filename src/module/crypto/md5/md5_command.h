#pragma once

#include "module/command.h"
#include "module/crypto/md5/md5_command_parse_result.h"
#include "module/crypto/md5/md5_preview_control.h"

namespace ra::module::crypto {

class MD5Command : public Command {
public:
    MD5Command(const MD5CommandParseResult& parse_result);

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    MD5CommandParseResult parse_result_;
    std::shared_ptr<MD5PreviewControl> preview_control_;
};

}