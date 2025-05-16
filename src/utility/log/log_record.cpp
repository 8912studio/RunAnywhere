#include "utility/log/log_record.h"
#include "utility/log/log_manager.h"

namespace ra::utility {

LogRecord::LogRecord(const zaf::SourceLocation& source_location) {
    stream_ << source_location.FileName() << '(' << source_location.Line() << ')' << ' ';
}


LogRecord::~LogRecord() {
    LogManager::Instance().WriteLine(stream_.view());
}

}