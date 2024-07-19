#include <iostream>
#include <chrono>
#include <thread>
#include "common/TCPSocket.h"

#pragma comment(lib, "Ws2_32.lib")


void communicate(Socket::Client& client_socket) {
    std::cout << "-- Communicating with server --\n";
    
    std::string data_send;
    std::optional<std::string> data_recv;

    while (true) {
        std::cout << "MyDB > ";
        std::getline(std::cin, data_send);
        data_send += '\n';
        client_socket.send_data(data_send.c_str());
        
        data_recv = client_socket.recv_until('\n');
        if (data_recv.has_value()) {
            std::cout << "Response: " << *data_recv;
        }
        else {
            std::cout << "No response\n";
            break;
        }
    }
}


int main() {
    // WSAStartup with version 2.2 initializing variable wsaData
    WSADATA wsaData;
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        printf("WSAStartup failed: %d\n", res);
        return false;
    }

    Socket::Client client_socket;
    
    bool connect_res = client_socket.connect_to_server("localhost", "80");
    if (!connect_res) {
        std::cout << "-- Could not connect to server --\n";
        return 1;
    }

    communicate(client_socket);

    // Done
    client_socket.clear();
    WSACleanup();
    std::cout << "Done!\n";
}
