#include "module/common/general_input.h"
#include "module/active_path/active_path_modifying.h"

namespace ra::mod {

GeneralInput MakeGeneralInput(
    const context::DesktopContext& context,
    const GeneralOption& option) {

    if (!option.text.empty()) {

        if (option.treat_text_as_file) {
            return GeneralInput(std::filesystem::path(option.text));
        }
        else {
            return GeneralInput(option.text, option.text_encoding.value_or(TextEncoding::UTF8));
        }
    }
    else {

        context::ActivePath active_path;
        if (option.active_path_option) {

            active_path = active_path::ModifyActivePathByOption(
                context.active_path,
                *option.active_path_option);
        }
        else {

            active_path = context.active_path;
        }

        if (option.text_encoding) {
            return GeneralInput(active_path.GetPath().wstring(), *option.text_encoding);
        }
        else {
            return GeneralInput(active_path.GetPath());
        }
    }
}

}