#include "module/ai/gpt/network/socket_manager.h"

namespace ra::mod::ai::gpt {

SocketManager::SocketManager(std::shared_ptr<boost::asio::io_service> io_service) : 
    io_service_(std::move(io_service)) {

}


curl_socket_t SocketManager::Open(curlsocktype socket_type, const curl_sockaddr* address) {

    if ((socket_type != CURLSOCKTYPE_IPCXN) || (address->family != AF_INET)) {
        return CURL_SOCKET_BAD;
    }

    auto asio_socket = std::make_shared<boost::asio::ip::tcp::socket>(*io_service_);

    boost::system::error_code error;
    asio_socket->open(boost::asio::ip::tcp::v4(), error);

    if (error) {
        return CURL_SOCKET_BAD;
    }

    sockets_.insert(std::make_pair(asio_socket->native_handle(), asio_socket));
    return asio_socket->native_handle();
}


bool SocketManager::Close(curl_socket_t socket) {

    auto iterator = sockets_.find(socket);
    if (iterator == sockets_.end()) {
        return false;
    }

    auto asio_socket = iterator->second;
    sockets_.erase(iterator);

    boost::system::error_code error;
    asio_socket->close(error);
    return !error;
}


void SocketManager::Watch(curl_socket_t socket, Event event, const EventCallback& callback) {

    auto iterator = sockets_.find(socket);
    if (iterator == sockets_.end()) {
        return;
    }

    auto watcher = std::make_shared<SingleSocketWatcher>(iterator->second, event, callback);
    watchers_.insert(std::make_pair(socket, watcher));
    watcher->Start();
}


void SocketManager::StopWatching(curl_socket_t socket) {

    auto iterator = watchers_.find(socket);
    if (iterator == watchers_.end()) {
        return;
    }

    auto watcher = iterator->second;
    watcher->Stop();
    watchers_.erase(iterator);
}


SingleSocketWatcher::SingleSocketWatcher(
    std::shared_ptr<boost::asio::ip::tcp::socket> socket,
    curlion::SocketWatcher::Event event, 
    curlion::SocketWatcher::EventCallback callback)
    :
    socket_(std::move(socket)),
    event_(event),
    callback_(std::move(callback)) {

}


void SingleSocketWatcher::Start() {
    Watch();
}


void SingleSocketWatcher::Stop() {

    //Stop may be called inside an event callback, so a flag variable
    //is needed for stopping the watching.
    is_stopped_ = true;

    //The socket may be closed before calling Stop, check for
    //preventing exception.
    if (socket_->is_open()) {
        socket_->cancel();
    }
}


void SingleSocketWatcher::Watch() {

    bool watch_read_event = 
        (event_ == curlion::SocketWatcher::Event::Read) ||
        (event_ == curlion::SocketWatcher::Event::ReadWrite);

    bool watch_write_event = 
        (event_ == curlion::SocketWatcher::Event::Write) || 
        (event_ == curlion::SocketWatcher::Event::ReadWrite);

    if (watch_read_event) {

        auto bridge_callback = std::bind(&SingleSocketWatcher::EventTriggered,
            this->shared_from_this(),
            false,
            std::placeholders::_1,
            std::placeholders::_2);

        socket_->async_read_some(boost::asio::null_buffers(), bridge_callback);
    }

    if (watch_write_event) {

        auto bridge_callback = std::bind(&SingleSocketWatcher::EventTriggered,
            this->shared_from_this(),
            true,
            std::placeholders::_1,
            std::placeholders::_2);

        socket_->async_write_some(boost::asio::null_buffers(), bridge_callback);
    }
}


void SingleSocketWatcher::EventTriggered(
    bool can_write,
    const boost::system::error_code& error,
    std::size_t size) {

    if (error == boost::asio::error::operation_aborted) {
        return;
    }

    callback_(socket_->native_handle(), can_write);

    //End watching once stopped.
    if (!is_stopped_) {
        Watch();
    }
}

}