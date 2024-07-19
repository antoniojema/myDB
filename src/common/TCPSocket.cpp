#include "TCPSocket.h"

#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "Ws2_32.lib")

namespace Socket {

bool Client::connect_to_server(const char* ipv4, const char* port) {
    if (this->in_use) {
        return false;
    }

    int res;

    struct addrinfo hints, *addr_info;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the local address and port to be used by the server
    res = getaddrinfo(ipv4, port, &hints, &addr_info);
    if (res != 0) {
        printf("getaddrinfo failed: %d\n", res);
        return false;
    }

    this->_socket = socket(
        addr_info->ai_family,
        addr_info->ai_socktype,
        addr_info->ai_protocol
    );
    if (this->_socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        return false;
    }

    res = connect(this->_socket, addr_info->ai_addr, (int)addr_info->ai_addrlen);
    if (res == SOCKET_ERROR) {
        printf("Unable to connect to server: %d\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        closesocket(this->_socket);
        this->_socket = INVALID_SOCKET;
        return false;
    }

    this->in_use = true;
    return true;
}


bool Client::send_data(const char* data) {
    int res = send(this->_socket, data, (int)strlen(data), 0);
    if (res == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        this->clear();
        return false;
    }
}


bool Client::send_data(const std::string& data) {
    int res = send(this->_socket, data.c_str(), data.length(), 0);
    if (res == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        this->clear();
        return false;
    }
}

std::optional<std::string> Client::recv_until(char end) {
    auto line = this->get_line(end);
    if (line.has_value()) {
        return line;
    }

    int read_res = this->recv_and_store();
    if (read_res < 0) {
        std::cout << "-- Error receiving data from client --\n   Error code: " << read_res << '\n';
        return std::nullopt;
    }
    if (read_res == 0) {
        std::cout << "-- Connection finished --\n";
        return std::string("");
    }

    line = this->get_line(end);
    if (line.has_value()) {
        return line;
    }
    return std::nullopt;
}


int Client::recv_and_store() {
    int buff_remain = this->buffer.size() - this->used_buff;
    if (buff_remain == 0) {
        std::cout << "-- Buffer out of space --\n";
    }

    int rv = recv(this->_socket, &this->buffer[this->used_buff], BUFF_SIZE, 0);

    if (rv < 0) {
        return rv;
    }
    this->used_buff += rv;

    return rv;
}

std::optional<std::string> Client::get_line(char end) {
    size_t pos_end = (size_t)-1;
    for (size_t pos = 0; pos < this->used_buff; ++pos) {
        if (this->buffer[pos] == end) {
            pos_end = pos + 1;
            break;
        }
    }
    if (pos_end == -1) {
        return std::nullopt;
    }

    std::string line(pos_end, ' ');
    memcpy(&line[0], &this->buffer[0], pos_end);
    memmove(&this->buffer[0], &this->buffer[pos_end], this->buffer.size() - pos_end);
    this->used_buff -= pos_end;

    return line;
}


void Client::clear() {
    if (!this->in_use) return;

    closesocket(this->_socket);
    this->used_buff = 0;
    this->in_use = false;
}


bool Server::bind_and_listen(const char* port) {
    if (in_use) {
        return false;
    }

    int res;

    struct addrinfo hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    res = getaddrinfo(NULL, port, &hints, &addr_info);
    if (res != 0) {
        printf("getaddrinfo failed: %d\n", res);
        return false;
    }
    
    this->listen_socket = socket(
        addr_info->ai_family,
        addr_info->ai_socktype,
        addr_info->ai_protocol
    );
    if (this->listen_socket == INVALID_SOCKET) {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        return false;
    }

    // Setup the TCP listening socket
    res = bind(this->listen_socket, addr_info->ai_addr, (int)addr_info->ai_addrlen);
    if (res == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        closesocket(this->listen_socket);
        return false;
    }

    if (listen(this->listen_socket, SOMAXCONN) == SOCKET_ERROR) {
        printf("listen failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(addr_info);
        closesocket(this->listen_socket);
        return false;
    }

    this->in_use = true;
    return true;
}

Client Server::get_client() {
    SOCKET client_socket = accept(this->listen_socket, NULL, NULL);
    if (client_socket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(client_socket);
        return false;
    }
    return Client{client_socket};
}

void Server::clear() {
    freeaddrinfo(addr_info);
    closesocket(this->listen_socket);

    addr_info = nullptr;
    this->listen_socket = INVALID_SOCKET;
    this->in_use = false;
}

} // namespace Socket
