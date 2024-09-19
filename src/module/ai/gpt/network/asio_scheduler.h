#pragma once

#include <thread>
#include <boost/asio.hpp>
#include <zaf/rx/scheduler.h>

namespace ra::mod::ai::gpt {

class ASIOScheduler : public zaf::Scheduler {
public:
    explicit ASIOScheduler(std::shared_ptr<boost::asio::io_service> io_service);
    ~ASIOScheduler();

    void Schedule(zaf::Work work) override;

private:
    std::unique_ptr<std::thread> thread_;
    std::shared_ptr<boost::asio::io_service> io_service_;
    std::unique_ptr<boost::asio::io_service::work> work_;
};

}