#pragma once

#include <filesystem>

namespace ra::module::user_defined {

class BundleMeta {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<BundleMeta>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetBundleID(const std::wstring& id) {
            result_->bundle_id_ = id;
        }

        std::shared_ptr<BundleMeta> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<BundleMeta> result_;
    };

public:
    BundleMeta() = default;

    BundleMeta(const BundleMeta&) = delete;
    BundleMeta& operator=(const BundleMeta&) = delete;

    const std::wstring& BundleID() const {
        return bundle_id_;
    }

private:
    std::wstring bundle_id_;
};

}