#pragma once

#include <sstream>
#include <zaf/base/non_copyable.h>
#include <zaf/base/source_location.h>

namespace ra::utility {

class LogRecord : public zaf::NonCopyableNonMovable {
public:
    explicit LogRecord(const zaf::SourceLocation& source_location);
    ~LogRecord();

    template<typename T>
    LogRecord& operator<<(T&& value) {
        stream_ << std::forward<T>(value);
        return *this;
    }

private:
    std::ostringstream stream_;
};

}