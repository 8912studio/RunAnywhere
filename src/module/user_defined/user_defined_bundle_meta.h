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

        void SetFilePath(const std::filesystem::path& file_path) {
            result_->file_path_ = file_path;
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

    const std::filesystem::path& FilePath() const {
        return file_path_;
    }

private:
    std::filesystem::path file_path_;
};

}