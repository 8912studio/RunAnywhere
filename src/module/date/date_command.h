#pragma once

#include "module/command.h"
#include "module/date/date_command_parse_result.h"
#include "module/date/date_preview_control.h"

namespace ra::module::date {

class DateCommand : public Command {
public:
    DateCommand(const DateCommandParseResult& parse_result);

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    DateCommandParseResult parse_result_;
    std::shared_ptr<DatePreviewControl> preview_control_;
};

}