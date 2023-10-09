#include "module/chat_gpt/comm/open_ai_client.h"
#include <zaf/rx/creation.h>
#include "module/chat_gpt/comm/asio_scheduler.h"
#include "module/chat_gpt/comm/socket_manager.h"
#include "module/chat_gpt/comm/socket_timer.h"

namespace ra::mod::chat_gpt::comm {

class OpenAIClientImpl {
public:
    curlion::ConnectionManager& GetConnectionManager() {

        std::call_once(once_flag_, [this]() {

            io_service_ = std::make_shared<boost::asio::io_service>();

            socket_manager_ = std::make_shared<SocketManager>(io_service_);
            socket_timer_ = std::make_shared<SocketTimer>(io_service_);
            scheduler_ = std::make_unique<ASIOScheduler>(io_service_);

            connction_manager_ = std::make_unique<curlion::ConnectionManager>(
                socket_manager_,
                socket_manager_,
                socket_timer_);
        });

        return *connction_manager_;
    }

private:
    std::once_flag once_flag_;
    std::shared_ptr<boost::asio::io_service> io_service_;
    std::shared_ptr<SocketManager> socket_manager_;
    std::shared_ptr<SocketTimer> socket_timer_;
    std::shared_ptr<ASIOScheduler> scheduler_;
    std::unique_ptr<curlion::ConnectionManager> connction_manager_;
};


OpenAIClient::OpenAIClient() : impl_(std::make_unique<OpenAIClientImpl>()) {

}


OpenAIClient::~OpenAIClient() {

}


zaf::Observable<ChatCompletion> OpenAIClient::CreateChatCompletion() {

    auto connection = std::make_shared<curlion::HttpConnection>();
    connection->SetUrl("http://www.qq.com");
    connection->SetFinishedCallback([](const std::shared_ptr<curlion::Connection>& connection) {
    
        auto result = connection->GetResult();
        auto response_body = connection->GetResponseBody();
        int x = 0;
    });

    impl_->GetConnectionManager().StartConnection(connection);
    return zaf::rx::Just(ChatCompletion{});
}

}