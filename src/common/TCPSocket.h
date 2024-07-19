#pragma once

#include <WinSock2.h>
#include <string_view>
#include <optional>
#include <vector>

#define BUFF_SIZE 1048576

namespace Socket {

class Client {
public:
    Client() : buffer(BUFF_SIZE) {}
    Client(SOCKET socket) : buffer(BUFF_SIZE), _socket(socket) {this->in_use = true;}
    Client(const char* ipv4, const char* port) : buffer(BUFF_SIZE) {this->connect_to_server(ipv4, port);}

    bool connect_to_server(const char* ipv4, const char* port);

    bool send_data(const std::string& data);
    bool send_data(const char* data);

    std::optional<std::string> recv_until(char end);

    void clear();

private:
    std::vector<char> buffer;
    size_t used_buff = 0;

    SOCKET _socket = INVALID_SOCKET;
    bool in_use = false;

    int recv_and_store();
    std::optional<std::string> get_line(char end);
};


class Server {
public:
    Server() = default;
    Server(const char* port) {this->bind_and_listen(port);}
    
    bool bind_and_listen(const char* port);
    Client get_client();

    void clear();

private:
    struct addrinfo* addr_info = nullptr;
    SOCKET listen_socket = INVALID_SOCKET;
    bool in_use = false;
};


} // namespace Socket

