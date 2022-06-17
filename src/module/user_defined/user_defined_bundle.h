#pragma once

#include <vector>
#include "module/user_defined/user_defined_bundle_meta.h"
#include "module/user_defined/user_defined_entry.h"

namespace ra::module::user_defined {

class UserDefinedBundle {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<UserDefinedBundle>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetMeta(const std::shared_ptr<UserDefinedBundleMeta>& meta) {
            result_->meta_ = meta;
        }

        void AddEntry(const std::shared_ptr<UserDefinedEntry>& entry) {
            result_->entries_.push_back(entry);
        }

        std::shared_ptr<UserDefinedBundle> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<UserDefinedBundle> result_;
    };

public:
    UserDefinedBundle() = default;

    UserDefinedBundle(const UserDefinedBundle&) = delete;
    UserDefinedBundle& operator=(const UserDefinedBundle&) = delete;

    std::shared_ptr<UserDefinedBundleMeta> Meta() const {
        return meta_;
    }

    const std::vector<std::shared_ptr<UserDefinedEntry>>& Entries() const {
        return entries_;
    }

private:
    std::shared_ptr<UserDefinedBundleMeta> meta_;
    std::vector<std::shared_ptr<UserDefinedEntry>> entries_;
};

}