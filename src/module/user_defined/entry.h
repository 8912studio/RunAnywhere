#pragma once

#include <string>
#include "module/user_defined/bundle_meta.h"
#include "module/user_defined/show_window_option.h"

namespace ra::module::user_defined {

class Entry {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<Entry>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetBundleMeta(const std::shared_ptr<BundleMeta>& meta) {
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

        void SetWorkingDirectory(const std::wstring& working_directory) {
            result_->working_directory_ = working_directory;
        }

        void SetShowWindowOption(ShowWindowOption option) {
            result_->show_window_option_ = option;
        }

        std::shared_ptr<Entry> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<Entry> result_;
    };

public:
    Entry() = default;

    Entry(const Entry&) = delete;
    Entry& operator=(const Entry&) = delete;

    const std::shared_ptr<BundleMeta>& BundleMeta() const {
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

    const std::wstring& WorkingDirectory() const {
        return working_directory_;
    }

    ShowWindowOption ShowWindowOption() const {
        return show_window_option_;
    }

private:
    std::shared_ptr<user_defined::BundleMeta> bundle_meta_;
    std::wstring keyword_;
    std::wstring command_;
    std::wstring description_;
    std::wstring working_directory_;
    user_defined::ShowWindowOption show_window_option_{ ShowWindowOption::Normal };
};

}