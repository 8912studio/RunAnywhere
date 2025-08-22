#pragma once

#include <optional>
#include <thread>
#include <boost/asio.hpp>
#include <zaf/rx/scheduler/scheduler.h>

namespace ra::mod::ai::gpt {

class ASIOScheduler : public zaf::rx::Scheduler {
public:
    explicit ASIOScheduler(std::shared_ptr<boost::asio::io_context> io_context);
    ~ASIOScheduler();

    void ScheduleWork(zaf::Closure work) override;

    std::shared_ptr<zaf::rx::Disposable> ScheduleDelayedWork(
        std::chrono::steady_clock::duration delay,
        zaf::Closure work) override;

private:
    std::unique_ptr<std::thread> thread_;
    std::shared_ptr<boost::asio::io_context> io_context_;

    using WorkGurad = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
    std::optional<WorkGurad> work_guard_;
};

}