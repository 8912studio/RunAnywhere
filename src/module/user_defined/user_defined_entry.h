#pragma once

#include <string>
#include "module/user_defined/user_defined_bundle_meta.h"

namespace ra::module::user_defined {

class UserDefinedEntry {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<UserDefinedEntry>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetBundleMeta(const std::shared_ptr<UserDefinedBundleMeta>& meta) {
            result_->bundle_meta_ = meta;
        }

        void SetKeyword(const std::wstring& keyword) {
            result_->keyword_ = keyword;
        }

        void SetCommand(const std::wstring& command) {
            result_->command_ = command;
        }

        void SetDescription(const std::wstring& description) {
            result_->description_ = description;
        }

        std::shared_ptr<UserDefinedEntry> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<UserDefinedEntry> result_;
    };

public:
    UserDefinedEntry() = default;

    UserDefinedEntry(const UserDefinedEntry&) = delete;
    UserDefinedEntry& operator=(const UserDefinedEntry&) = delete;

    const std::shared_ptr<UserDefinedBundleMeta>& BundleMeta() const {
        return bundle_meta_;
    }

    const std::wstring& Keyword() const {
        return keyword_;
    }

    const std::wstring& Command() const {
        return command_;
    }

    const std::wstring& Description() const {
        return description_;
    }

private:
    std::shared_ptr<UserDefinedBundleMeta> bundle_meta_;
    std::wstring keyword_;
    std::wstring command_;
    std::wstring description_;
};

}