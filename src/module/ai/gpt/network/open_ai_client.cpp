#include "module/ai/gpt/network/open_ai_client.h"
#include <zaf/application.h>
#include <zaf/base/error/invalid_data_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/timer.h>
#include "module/ai/gpt/network/asio_scheduler.h"
#include "module/ai/gpt/network/error.h"
#include "module/ai/gpt/network/socket_manager.h"
#include "module/ai/gpt/network/socket_timer.h"
#include "module/tool/json/json_formatter.h"
#include "option/option_storage.h"

namespace ra::mod::ai::gpt {

class OpenAIClientImpl {
public:
    curlion::ConnectionManager& GetConnectionManager() {

        std::call_once(once_flag_, [this]() {

            curlion::SetLogger([](const std::string& log) {
                OutputDebugStringA(log.c_str());
            });

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
    const std::vector<const Message*>& messages) {

    auto url = zaf::ToUTF8String(option::OptionStorage::Instance().OpenAIAPIServer());
    if (!url.empty()) {
        if (url.back() != '/') {
            url.append(1, '/');
        }
        url.append("v1/chat/completions");
    }

    auto api_key = zaf::ToUTF8String(option::OptionStorage::Instance().OpenAIAPIKey());

    auto connection = std::make_shared<curlion::HttpConnection>();
    connection->SetConnectTimeoutInMilliseconds(5000);
    connection->SetIdleTimeoutInSeconds(60);
    connection->SetUrl(url);
    connection->SetRequestHeaders({
        { "Content-Type", "application/json" },
        { "Authorization", "Bearer " + api_key },
    });

    auto proxy = option::OptionStorage::Instance().Proxy();
    if (!proxy.empty()) {
        connection->SetProxy(zaf::ToUTF8String(proxy));
    }
    
    connection->SetUsePost(true);
    connection->SetRequestBody(CreateRequestBody(messages));

    curl_easy_setopt(connection->GetHandle(), CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);

    connection->SetVerbose(true);
    connection->SetDebugCallback([](
        const std::shared_ptr<curlion::Connection>& connection,
        curlion::Connection::DebugDataType data_type,
        const char* data,
        std::size_t size) {
    
        if (data_type == curlion::Connection::DebugDataType::Information ||
            data_type == curlion::Connection::DebugDataType::SentHeader ||
            data_type == curlion::Connection::DebugDataType::SentBody ||
            data_type == curlion::Connection::DebugDataType::ReceivedHeader ||
            data_type == curlion::Connection::DebugDataType::ReceivedBody) {

            std::string content(data, size);
            OutputDebugStringA(content.c_str());
        }
    });

    zaf::ReplaySubject<ChatCompletion> subject;
    connection->SetFinishedCallback([observer = subject.AsObserver()](
        const std::shared_ptr<curlion::Connection>& connection) {
    
        auto curl_code = connection->GetResult();
        if (curl_code != CURLE_OK) {
            observer.OnError(CURLError{ 
                curl_code,
                connection->GetError(), 
                ZAF_SOURCE_LOCATION() 
            }); 
            return;
        }

        auto http_code = connection->GetResponseCode();
        if (http_code != 200) {
            observer.OnError(HTTPError{
                static_cast<int>(http_code),
                ParseErrorMessage(connection->GetResponseBody()),
                ZAF_SOURCE_LOCATION()
            });
            return;
        }

        const auto& response_body = connection->GetResponseBody();
        auto chat_completion = ParseChatCompletion(response_body);

        if (chat_completion) {
            observer.OnNext(*chat_completion);
            observer.OnCompleted();
        }
        else {
            observer.OnError(zaf::InvalidDataError{ ZAF_SOURCE_LOCATION() });
        }
    });

    auto error_condition = impl_->GetConnectionManager().StartConnection(connection);
    if (error_condition) {

        std::error_code error_code(error_condition.value(), error_condition.category());
        subject.AsObserver().OnError(CURLMultiSocketError{
            static_cast<CURLMcode>(error_condition.value()), 
            ZAF_SOURCE_LOCATION(),
        });
    }

    return subject.AsObservable().ObserveOn(zaf::Scheduler::Main());
}


zaf::Observable<ChatCompletion> OpenAIClient::CreateMockChatCompletion() {

    zaf::ReplaySubject<ChatCompletion> subject;

    zaf::Application::Instance().Subscriptions() +=
        zaf::rx::Timer(std::chrono::seconds(3), zaf::Scheduler::Main()).Subscribe(
            [observer = subject.AsObserver()](int) {
    
        std::wstring mock_response =
LR"(**Asterisk (*)**: )";

        Message message{ RoleAssistant, mock_response };

        TokenUsage token_usage;
        token_usage.prompt_tokens = 17;
        token_usage.completion_tokens = 29;
        token_usage.total_tokens = 46;

        observer.OnNext(ChatCompletion{ std::move(message), token_usage });
        observer.OnCompleted();
    });
    
    return subject.AsObservable();
}


std::string OpenAIClient::CreateRequestBody(const std::vector<const Message*>& messages) {

    boost::json::object root;
    root["model"] = "gpt-4o-mini";
    root["temperature"] = 0.6f;

    boost::json::array message_array;
    for (const auto& each_message : messages) {
        
        boost::json::object message_item;
        message_item["role"] = zaf::ToUTF8String(each_message->Role());
        message_item["content"] = zaf::ToUTF8String(each_message->Content());
        message_array.push_back(std::move(message_item));
    }
    root["messages"] = std::move(message_array);

    tool::json::JSONPrimitiveFormatter json_formatter;
    return json_formatter.Format(root);
}


std::optional<ChatCompletion> OpenAIClient::ParseChatCompletion(const std::string& response) {

    try {

        auto json_root = boost::json::parse(response);

        auto message = ParseMessage(json_root);
        auto token_usage = ParseTokenUsage(json_root);
        
        return ChatCompletion(std::move(message), token_usage);
    }
    catch (const std::exception&) {
        return std::nullopt;
    }
}


Message OpenAIClient::ParseMessage(const boost::json::value& json_root) {

    const auto& json_choice_array = json_root.at("choices").as_array();
    const auto& json_choice = json_choice_array.at(0).as_object();

    const auto& json_message = json_choice.at("message").as_object();
    const auto& json_role = json_message.at("role");
    const auto& json_content = json_message.at("content");

    auto role = boost::json::value_to<std::string>(json_role);
    auto content = boost::json::value_to<std::string>(json_content);

    return Message{ zaf::FromUTF8String(role), zaf::FromUTF8String(content) };
}


std::optional<TokenUsage> OpenAIClient::ParseTokenUsage(const boost::json::value& json_root) {

    try {

        const auto& json_usage = json_root.at("usage").as_object();

        TokenUsage result;
        result.prompt_tokens = boost::json::value_to<std::size_t>(json_usage.at("prompt_tokens"));
        result.completion_tokens =
            boost::json::value_to<std::size_t>(json_usage.at("completion_tokens"));
        result.total_tokens = boost::json::value_to<std::size_t>(json_usage.at("total_tokens"));
        return result;
    }
    catch (...) {
        return std::nullopt;
    }
}


std::string OpenAIClient::ParseErrorMessage(const std::string& response) {

    try {
        auto json_root = boost::json::parse(response);
        const auto& error = json_root.at("error").as_object();
        const auto& message = error.at("message");
        return boost::json::value_to<std::string>(message);
    }
    catch (const std::exception&) {
        return {};
    }
}

}