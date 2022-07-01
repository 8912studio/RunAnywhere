#pragma once

#include <filesystem>

namespace ra::module::user_defined {

class UserDefinedBundleMeta {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<UserDefinedBundleMeta>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetBundleID(const std::wstring& id) {
            result_->bundle_id_ = id;
        }

        std::shared_ptr<UserDefinedBundleMeta> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<UserDefinedBundleMeta> result_;
    };

public:
    UserDefinedBundleMeta() = default;

    UserDefinedBundleMeta(const UserDefinedBundleMeta&) = delete;
    UserDefinedBundleMeta& operator=(const UserDefinedBundleMeta&) = delete;

    const std::wstring& BundleID() const {
        return bundle_id_;
    }

private:
    std::wstring bundle_id_;
};

}