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
#include "module/ai/gpt/network/response_parsing.h"
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


zaf::Observable<ChatResult> OpenAIClient::CreateChatCompletion(
    const std::vector<Message>& messages) {

    return CreateMockChatCompletion();

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

    zaf::ReplaySubject<ChatResult> subject;
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
            observer.OnNext(ChatResult{ std::move(*chat_completion), response_body });
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


zaf::Observable<ChatResult> OpenAIClient::CreateMockChatCompletion() {

    zaf::ReplaySubject<ChatResult> subject;

    zaf::Application::Instance().Subscriptions() +=
        zaf::rx::Timer(std::chrono::seconds(3), zaf::Scheduler::Main()).Subscribe(
            [observer = subject.AsObserver()](int) {
    
        std::wstring mock_response =
LR"(Yes, there are several libraries and tools available that can help you generate SQL queries programmatically. Here are some popular ones across different programming languages:

### Python
1. **SQLAlchemy**: An SQL toolkit and Object-Relational Mapping (ORM) system that allows you to construct SQL queries using Python objects.
   - Website: [SQLAlchemy](https://www.sqlalchemy.org/)

2. **Peewee**: A small, expressive ORM that provides a simple way to interact with databases and generate SQL queries.
   - Website: [Peewee](http://docs.peewee-orm.com/)

3. **Pandas**: While primarily a data manipulation library, it has built-in capabilities to generate SQL from DataFrames using the `to_sql` method.
   - Website: [Pandas](https://pandas.pydata.org/)

### JavaScript/Node.js
1. **Knex.js**: A SQL query builder for Node.js that allows you to construct queries using a fluent API.
   - Website: [Knex.js](http://knexjs.org/)

2. **Sequelize**: A promise-based Node.js ORM that supports multiple SQL dialects and allows for easy query generation.
   - Website: [Sequelize](https://sequelize.org/)

### Ruby
1. **Active Record**: The ORM framework that comes with Ruby on Rails, which allows you to create SQL queries using Ruby syntax.
   - Website: [Active Record](https://guides.rubyonrails.org/active_record_basics.html)

2. **Sequel**: A simple, flexible, and powerful SQL database toolkit for Ruby.
   - Website: [Sequel](http://sequel.jeremyevans.net/)

### PHP
1. **Laravel Eloquent**: The ORM that comes with the Laravel framework, which provides a simple ActiveRecord implementation for generating SQL queries.
   - Website: [Eloquent](https://laravel.com/docs/eloquent)

2. **Doctrine DBAL**: A database abstraction layer that provides a powerful query builder for PHP.
   - Website: [Doctrine DBAL](https://www.doctrine-project.org/projects/doctrine-dbal/en/latest/index.html)

### Java
1. **JOOQ**: A fluent API for typesafe SQL query construction and execution in Java.
   - Website: [JOOQ](https://www.jooq.org/)

2. **Hibernate**: An ORM framework for Java that allows you to generate SQL queries based on Java objects.
   - Website: [Hibernate](https://hibernate.org/)

### C#
1. **Entity Framework**: An ORM framework for .NET that allows you to work with databases using .NET objects, generating SQL queries automatically.
   - Website: [Entity Framework](https://docs.microsoft.com/en-us/ef/)

2. **Dapper**: A simple object mapper for .NET that allows you to execute SQL queries directly and map results to C# objects.
   - Website: [Dapper](https://github.com/DapperLib/Dapper)

### General Tools
1. **SQL Builders**: Many SQL builders are available in various languages that allow you to construct SQL queries using a programmatic interface, such as:
   - **SQL::Abstract** (Perl)
   - **sql-bricks** (JavaScript)

These libraries can help you generate SQL queries dynamically, making it easier to work with databases in your applications. Depending on your programming language and requirements, you can choose the one that best fits your needs.)";

        Message message{ RoleAssistant, mock_response };

        TokenUsage token_usage;
        token_usage.prompt_tokens = 17;
        token_usage.completion_tokens = 29;
        token_usage.total_tokens = 46;

        observer.OnNext(ChatResult{
            ChatCompletion{ std::move(message), token_usage }, 
            zaf::ToUTF8String(mock_response), 
        });
        observer.OnCompleted();
    });
    
    return subject.AsObservable();
}


std::string OpenAIClient::CreateRequestBody(const std::vector<Message>& messages) {

    boost::json::object root;
    root["model"] = "gpt-4o-mini";
    root["temperature"] = 0.6f;

    boost::json::array message_array;
    for (const auto& each_message : messages) {
        
        boost::json::object message_item;
        message_item["role"] = zaf::ToUTF8String(each_message.Role());
        message_item["content"] = zaf::ToUTF8String(each_message.Content());
        message_array.push_back(std::move(message_item));
    }
    root["messages"] = std::move(message_array);

    tool::json::JSONPrimitiveFormatter json_formatter;
    return json_formatter.Format(root);
}

}