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

        void AddGlobalProperty(const std::wstring& key, const std::wstring& value) {
            result_->global_properties_.emplace_back(key, value);
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

    const std::vector<std::pair<std::wstring, std::wstring>>& GlobalProperties() const {
        return global_properties_;
    }

private:
    std::wstring bundle_id_;
    std::vector<std::pair<std::wstring, std::wstring>> global_properties_;
};

}