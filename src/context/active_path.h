#pragma once

#include <filesystem>

namespace ra::context {

class ActivePath {
public:
	ActivePath() = default;

	explicit ActivePath(const std::filesystem::path& path) : path_(path) {

	}

	ActivePath(const std::filesystem::path& path, const std::filesystem::path& workspace_path) :
		path_(path),
		workspace_path_(workspace_path) {

	}

	const std::filesystem::path& GetPath() const {
		return path_;
	}

	const std::filesystem::path& GetWorkspacePath() const {
		return workspace_path_;
	}

	bool IsEmpty() const {
		return path_.empty();
	}

	bool HasWorkspacePath() const {
		return !workspace_path_.empty();
	}

private:
	std::filesystem::path path_;
	std::filesystem::path workspace_path_;
};

}