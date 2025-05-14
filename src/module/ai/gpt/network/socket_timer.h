#pragma once

#include <memory>
#include <boost/asio.hpp>
#include <curlion.h>

namespace ra::mod::ai::gpt {

class SocketTimer : public curlion::Timer {
public:
    explicit SocketTimer(std::shared_ptr<boost::asio::io_context> io_context);

    void Start(long timeout_ms, const std::function<void()>& callback) override;
    void Stop() override;

private:
    void TimerTriggered(
        const std::function<void()>& callback, 
        const boost::system::error_code& error);

private:
    std::shared_ptr<boost::asio::io_context> io_context_;
    std::unique_ptr<boost::asio::deadline_timer> timer_;
};

}