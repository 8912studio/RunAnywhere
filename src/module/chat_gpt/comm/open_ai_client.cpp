#include "module/chat_gpt/comm/open_ai_client.h"
#include <boost/json.hpp>
#include <zaf/application.h>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/timer.h>
#include "module/chat_gpt/comm/asio_scheduler.h"
#include "module/chat_gpt/comm/error.h"
#include "module/chat_gpt/comm/socket_manager.h"
#include "module/chat_gpt/comm/socket_timer.h"
#include "option/option_storage.h"

namespace ra::mod::chat_gpt::comm {

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
    const std::vector<Message>& messages) {

    return CreateMockChatCompletion(messages);

    zaf::ReplaySubject<ChatCompletion> subject;

    auto api_key = zaf::ToUTF8String(option::OptionStorage::Instance().OpenAIAPIKey());

    auto connection = std::make_shared<curlion::HttpConnection>();
    connection->SetConnectTimeoutInMilliseconds(5000);
    connection->SetIdleTimeoutInSeconds(60);
    connection->SetUrl("https://api.openai.com/v1/chat/completions");
    connection->SetRequestHeaders({
        { "Content-Type", "application/json" },
        { "Authorization", "Bearer " + api_key },
    });

    auto proxy = option::OptionStorage::Instance().Proxy();
    if (!proxy.empty()) {
        connection->SetProxy(zaf::ToUTF8String(proxy));
    }
    
    connection->SetUsePost(1);
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

    connection->SetFinishedCallback([observer = subject.AsObserver(), messages](
        const std::shared_ptr<curlion::Connection>& connection) {
    
        auto curl_code = connection->GetResult();
        if (curl_code != CURLE_OK) {
            observer.OnError(zaf::Error{ 
                std::error_code(curl_code, CURLErrorCategory()),
                connection->GetError()
            });
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

    zaf::Application::Instance().Subscriptions() +=
        zaf::rx::Timer(std::chrono::seconds(3), zaf::Scheduler::Main()).Subscribe(
            [observer = subject.AsObserver()](int) {
    
        std::wstring mock_response = LR"(To use CryptProtectData() in Windows, follow these steps:

1. Include the necessary header file: 
   ```
   #include <wincrypt.h>
   ```

2. Link against the `Crypt32.lib` library by adding it to your project settings.

3. Declare the necessary variables:
   ```cpp
   DATA_BLOB inputData;
   DATA_BLOB encryptedData;
   LPCWSTR description = L"Data description"; // optional description for encrypted data
   ```

4. Prepare the data to be encrypted:
   ```cpp
   // Convert your data to a byte array
   BYTE* dataToEncrypt = reinterpret_cast<BYTE*>(yourData);
   DWORD dataSize = sizeof(yourData);
   
   // Set the input data blob
   inputData.pbData = dataToEncrypt;
   inputData.cbData = dataSize;
   ```

5. Call CryptProtectData() to encrypt the data:
   ```cpp
   if (CryptProtectData(&inputData, description, NULL, NULL, NULL, 0, &encryptedData))
   {
       // Encrypted data is in encryptedData.pbData
       // Encrypted data size is in encryptedData.cbData
       // ...
       
       // Free the encrypted data when done
       LocalFree(encryptedData.pbData);
   }
   else
   {
       // Encryption failed
       DWORD errorCode = GetLastError();
       // Handle the error
   }
   ```

Note: CryptProtectData() uses the user's logon credentials to encrypt the data. The encrypted data can only be decrypted by the same user on the same machine.

Make sure to handle any errors that may occur during the encryption process and free any allocated memory appropriately.)";

        observer.OnNext(ChatCompletion(Message(RoleAssistant, mock_response)));
        observer.OnCompleted();
    });
    
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