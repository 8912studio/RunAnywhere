#pragma once

#include <map>
#include <boost/asio.hpp>
#include <curlion.h>

namespace ra::mod::chat_gpt::comm {

class SingleSocketWatcher;

class SocketManager : public curlion::SocketFactory, public curlion::SocketWatcher {
public:
    explicit SocketManager(std::shared_ptr<boost::asio::io_service>);

    curl_socket_t Open(curlsocktype socket_type, const curl_sockaddr* address) override;
    bool Close(curl_socket_t socket) override;

    void Watch(curl_socket_t socket, Event event, const EventCallback& callback) override;
    void StopWatching(curl_socket_t socket) override;

private:
    std::shared_ptr<boost::asio::io_service> io_service_;
    std::map<curl_socket_t, std::shared_ptr<boost::asio::ip::tcp::socket>> sockets_;
    std::map<curl_socket_t, std::shared_ptr<SingleSocketWatcher>> watchers_;
};


class SingleSocketWatcher : public std::enable_shared_from_this<SingleSocketWatcher> {
public:
    SingleSocketWatcher(
        std::shared_ptr<boost::asio::ip::tcp::socket> socket,
        curlion::SocketWatcher::Event event,
        curlion::SocketWatcher::EventCallback callback);

    void Start();
    void Stop();

private:
    void Watch();
    void EventTriggered(
        bool can_write,
        const boost::system::error_code& error,
        std::size_t size);

private:
    std::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    curlion::SocketWatcher::Event event_;
    curlion::SocketWatcher::EventCallback callback_;
    bool is_stopped_{};
};

}