#pragma once

#include "module/command.h"
#include "module/tool/md5/md5_command_parse_result.h"
#include "module/tool/md5/md5_preview_control.h"

namespace ra::module::tool::md5 {

class MD5Command : public Command {
public:
    MD5Command(const MD5CommandParseResult& parse_result);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    MD5CommandParseResult parse_result_;
    std::shared_ptr<MD5PreviewControl> preview_control_;
};

}