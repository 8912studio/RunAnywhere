#include "module/ai/gpt/network/asio_scheduler.h"

namespace ra::mod::chat_gpt::comm {

ASIOScheduler::ASIOScheduler(std::shared_ptr<boost::asio::io_service> io_service) : 
    io_service_(std::move(io_service)) {

    work_ = std::make_unique<boost::asio::io_service::work>(*io_service_);

    thread_ = std::make_unique<std::thread>([io_service = io_service_]() {
        io_service->run();
    });
}


ASIOScheduler::~ASIOScheduler() {

    work_.reset();

    if (thread_->joinable()) {
        thread_->join();
    }
}


void ASIOScheduler::Schedule(zaf::Work work) {

    io_service_->dispatch(work);
}

}