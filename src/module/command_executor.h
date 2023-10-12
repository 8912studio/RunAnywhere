#pragma once

namespace ra::mod {

enum class PostExecuteAction {
    Dispose,
    Preserve,
};


class ExecuteResult {
public:
    ExecuteResult(PostExecuteAction post_execute_action) :
        post_execute_action_(post_execute_action) {

    }

    PostExecuteAction PostExecuteAction() const {
        return post_execute_action_;
    }

private:
    mod::PostExecuteAction post_execute_action_{ mod::PostExecuteAction::Dispose };
};


class CommandExecutor {
public:
    CommandExecutor() = default;
    virtual ~CommandExecutor() = default;

    virtual ExecuteResult Execute() = 0;
};

}