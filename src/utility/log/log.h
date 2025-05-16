#pragma once

#include "utility/log/log_record.h"

#define RA_LOG() ra::utility::LogRecord{ ZAF_SOURCE_LOCATION() }