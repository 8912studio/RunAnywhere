#pragma once

#include <zaf/rx/observable.h>
#include "context/desktop_context.h"

namespace ra::context {

zaf::Observable<DesktopContext> DiscoverDesktopContext();

}