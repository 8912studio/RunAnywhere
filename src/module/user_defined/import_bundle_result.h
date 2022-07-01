#pragma once

namespace ra::module::user_defined {

class ImportBundleResult {
public:
    enum class Status {
        OK,
    };

public:
    Status status{ Status::OK };
};

}

