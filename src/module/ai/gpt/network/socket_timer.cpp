#include "module/ai/gpt/network/socket_timer.h"

namespace ra::mod::ai::gpt {

SocketTimer::SocketTimer(std::shared_ptr<boost::asio::io_context> io_context) :
    io_context_(std::move(io_context)) {

    timer_ = std::make_unique<boost::asio::deadline_timer>(*io_context_);
}


void SocketTimer::Start(long timeout_ms, const std::function<void()>& callback) {

    timer_->expires_from_now(boost::posix_time::milliseconds(timeout_ms));
    timer_->async_wait(std::bind(
        &SocketTimer::TimerTriggered, 
        this, 
        callback, 
        std::placeholders::_1));
}


void SocketTimer::Stop() {

    timer_->cancel();
}


void SocketTimer::TimerTriggered(
    const std::function<void()>& callback,
    const boost::system::error_code& error) {

    if (error == boost::asio::error::operation_aborted) {
        return;
    }

    callback();
}

}