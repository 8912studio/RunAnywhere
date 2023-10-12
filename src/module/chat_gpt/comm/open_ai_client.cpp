#include "module/chat_gpt/comm/open_ai_client.h"
#include <boost/json.hpp>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/string/encoding_conversion.h>
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

    return CreateMockChatCompletion(messages);

    zaf::ReplaySubject<ChatCompletion> subject;

    auto connection = std::make_shared<curlion::HttpConnection>();
    connection->SetUrl("https://api.openai.com/v1/chat/completions");
    connection->SetRequestHeaders({
        { "Content-Type", "application/json" },
        { "Authorization", "Bearer" },
    });
    //connection->SetProxy("http://127.0.0.1:12377");
    connection->SetUsePost(1);
    connection->SetRequestBody(CreateRequestBody(messages));

    curl_easy_setopt(connection->GetHandle(), CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);

    connection->SetVerbose(true);
    connection->SetDebugCallback([](
        const std::shared_ptr<curlion::Connection>& connection,
        curlion::Connection::DebugDataType data_type,
        const char* data,
        std::size_t size) {
    
        std::string content(data, size);
        OutputDebugStringA(content.c_str());
    });

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

        const auto& response_body = connection->GetResponseBody();
        auto chat_completion = ParseChatCompletion(response_body);

        if (chat_completion) {
            observer.OnNext(*chat_completion);
            observer.OnCompleted();
        }
        else {
            observer.OnError(zaf::Error(zaf::make_error_code(zaf::BasicErrc::InvalidValue)));
        }
    });

    auto error_condition = impl_->GetConnectionManager().StartConnection(connection);
    if (error_condition) {
        std::error_code error_code(error_condition.value(), error_condition.category());
        subject.AsObserver().OnError(zaf::Error(error_code));
    }

    return subject.AsObservable().ObserveOn(zaf::Scheduler::Main());
}


zaf::Observable<ChatCompletion> OpenAIClient::CreateMockChatCompletion(
    const std::vector<Message>& messages) {

    zaf::ReplaySubject<ChatCompletion> subject;

    std::wstring mock_response;
    if (messages.empty()) {
        mock_response = L"Input your question.";
    }
    else {
        mock_response = L"Here is the response to " + messages.back().Content();
    }

    subject.AsObserver().OnNext(ChatCompletion(Message(RoleAssistant, mock_response)));
    subject.AsObserver().OnCompleted();
    return subject.AsObservable();
}


std::string OpenAIClient::CreateRequestBody(const std::vector<Message>& messages) {

    boost::json::object root;
    root["model"] = "gpt-3.5-turbo";
    root["temperature"] = 0.6f;

    boost::json::array message_array;
    for (const auto& each_message : messages) {
        
        boost::json::object message_item;
        message_item["role"] = zaf::ToUTF8String(each_message.Role());
        message_item["content"] = zaf::ToUTF8String(each_message.Content());
        message_array.push_back(std::move(message_item));
    }
    root["messages"] = std::move(message_array);

    return boost::json::serialize(root);
}


std::optional<ChatCompletion> OpenAIClient::ParseChatCompletion(const std::string& response) {

    try {

        auto json_root = boost::json::parse(response);

        const auto& json_choice_array = json_root.at("choices").as_array();
        const auto& json_choice = json_choice_array.at(0).as_object();

        const auto& json_message = json_choice.at("message").as_object();
        const auto& json_role = json_message.at("role");
        const auto& json_content = json_message.at("content");

        auto role = boost::json::value_to<std::string>(json_role);
        auto content = boost::json::value_to<std::string>(json_content);

        Message message(zaf::FromUTF8String(role), zaf::FromUTF8String(content));
        return ChatCompletion(std::move(message));
    }
    catch (const std::exception&) {
        return std::nullopt;
    }
}

}