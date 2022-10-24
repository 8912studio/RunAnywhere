#include "module/user_defined/show_window_option.h"
#include <zaf/object/enum_definition.h>

namespace ra::module::user_defined{

ZAF_DEFINE_ENUM(ShowWindowOption)
ZAF_DEFINE_ENUM_VALUE(Normal)
ZAF_DEFINE_ENUM_VALUE(Hide)
ZAF_DEFINE_ENUM_END

}