#pragma once

#include <string>
#include <zaf/rx/observable.h>
#include <zaf/rx/subject.h>

namespace ra::mod::chat_gpt {

class Round {
public:
    Round(std::uint64_t id, std::wstring question, zaf::Observable<std::wstring> answer);
    ~Round();

    std::uint64_t ID() const {
        return id_;
    }

    const std::wstring& Question() const {
        return question_;
    }

    zaf::Observable<std::wstring> Answer() const {
        return answer_;
    }

    void Remove();

    zaf::Observable<std::uint64_t> RemoveEvent() const {
        return remove_event_.AsObservable();
    }

private:
    std::uint64_t id_{};
    std::wstring question_;
    zaf::Observable<std::wstring> answer_;
    zaf::Subject<std::uint64_t> remove_event_;
};

}
