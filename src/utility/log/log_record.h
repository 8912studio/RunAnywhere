#pragma once

#include <sstream>
#include <zaf/base/non_copyable.h>

namespace ra::utility {

class LogRecord : public zaf::NonCopyableNonMovable {
public:
    LogRecord();
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