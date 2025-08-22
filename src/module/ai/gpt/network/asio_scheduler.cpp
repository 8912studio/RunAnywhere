#include "module/ai/gpt/network/asio_scheduler.h"
#include <zaf/base/error/invalid_operation_error.h>

namespace ra::mod::ai::gpt {

ASIOScheduler::ASIOScheduler(std::shared_ptr<boost::asio::io_context> io_context) :
    io_context_(std::move(io_context)) {

    work_guard_.emplace(boost::asio::make_work_guard(*io_context_));

    thread_ = std::make_unique<std::thread>([io_context = io_context_]() {
        io_context->run();
    });
}


ASIOScheduler::~ASIOScheduler() {

    work_guard_.reset();

    if (thread_->joinable()) {
        thread_->join();
    }
}


void ASIOScheduler::ScheduleWork(zaf::Closure work) {
    boost::asio::post(*io_context_, std::move(work));
}


std::shared_ptr<zaf::rx::Disposable> ASIOScheduler::ScheduleDelayedWork(
    std::chrono::steady_clock::duration delay, 
    zaf::Closure work) {

    throw zaf::InvalidOperationError{};
}

}