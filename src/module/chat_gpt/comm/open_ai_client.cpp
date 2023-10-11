#include "module/chat_gpt/comm/open_ai_client.h"
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/subject.h>
#include "module/chat_gpt/comm/asio_scheduler.h"
#include "module/chat_gpt/comm/error.h"
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


zaf::Observable<ChatCompletion> OpenAIClient::CreateChatCompletion(
    const std::vector<Message>& messages) {

    zaf::ReplaySubject<ChatCompletion> subject;

    auto connection = std::make_shared<curlion::HttpConnection>();
    connection->SetUrl("http://www.qq.com");
    connection->SetFinishedCallback([observer = subject.AsObserver(), messages](
        const std::shared_ptr<curlion::Connection>& connection) {
    
        auto curl_code = connection->GetResult();
        if (curl_code != CURLE_OK) {
            observer.OnError(zaf::Error(std::error_code(curl_code, CURLErrorCategory())));
            return;
        }

        auto http_code = connection->GetResponseCode();
        if (http_code != 200) {
            observer.OnError(zaf::Error(std::error_code(http_code, HTTPErrorCategory())));
            return;
        }

        auto response_body = connection->GetResponseBody();

        std::wstring mock_response;
        if (messages.empty()) {
            mock_response = L"Input your question.";
        }
        else {
            mock_response = L"Here is the response to " + messages.back().Content();
        }

        observer.OnNext(ChatCompletion(Message(RoleAssistant, mock_response)));
        observer.OnCompleted();
    });

    auto error_condition = impl_->GetConnectionManager().StartConnection(connection);
    if (error_condition) {
        std::error_code error_code(error_condition.value(), error_condition.category());
        subject.AsObserver().OnError(zaf::Error(error_code));
    }

    return subject.AsObservable().ObserveOn(zaf::Scheduler::Main());
}

}