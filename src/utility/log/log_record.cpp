#include "utility/log/log_record.h"
#include "utility/log/log_manager.h"

namespace ra::utility {

LogRecord::LogRecord() {

}


LogRecord::~LogRecord() {
    LogManager::Instance().WriteLine(stream_.view());
}

}