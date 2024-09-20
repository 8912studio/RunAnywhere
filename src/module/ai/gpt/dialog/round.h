#pragma once

#include <memory>
#include <string>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subject.h>
#include "module/ai/gpt/network/chat_completion.h"

namespace ra::mod::ai::gpt {

class Round : public std::enable_shared_from_this<Round>, zaf::NonCopyableNonMovable {
public:
    Round(std::uint64_t id, std::wstring question, zaf::Observable<ChatCompletion> answer);
    ~Round();

    std::uint64_t ID() const {
        return id_;
    }

    const std::wstring& Question() const {
        return question_;
    }

    zaf::Observable<ChatCompletion> Answer() const {
        return answer_;
    }

    void Remove();
    void Retry();

    zaf::Observable<std::uint64_t> RemoveEvent() const {
        return remove_event_.AsObservable();
    }

    zaf::Observable<std::shared_ptr<Round>> RetryEvent() const {
        return retry_event_.AsObservable();
    }

private:
    std::uint64_t id_{};
    std::wstring question_;
    zaf::Observable<ChatCompletion> answer_;
    zaf::Subject<std::uint64_t> remove_event_;
    zaf::Subject<std::shared_ptr<Round>> retry_event_;
};

}
