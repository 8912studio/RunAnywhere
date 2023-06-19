#pragma once

#include <vector>
#include "module/user_defined/bundle_meta.h"
#include "module/user_defined/entry.h"

namespace ra::mod::user_defined {

class Bundle {
public:
    class Builder {
    public:
        Builder() : result_(std::make_shared<Bundle>()) {

        }

        Builder(const Builder&) = delete;
        Builder& operator=(const Builder&) = delete;

        void SetMeta(const std::shared_ptr<BundleMeta>& meta) {
            result_->meta_ = meta;
        }

        void AddEntry(const std::shared_ptr<Entry>& entry) {
            result_->entries_.push_back(entry);
        }

        std::shared_ptr<Bundle> Build() {
            return std::move(result_);
        }

    private:
        std::shared_ptr<Bundle> result_;
    };

public:
    Bundle() = default;

    Bundle(const Bundle&) = delete;
    Bundle& operator=(const Bundle&) = delete;

    std::shared_ptr<BundleMeta> Meta() const {
        return meta_;
    }

    const std::vector<std::shared_ptr<Entry>>& Entries() const {
        return entries_;
    }

private:
    std::shared_ptr<BundleMeta> meta_;
    std::vector<std::shared_ptr<Entry>> entries_;
};

}