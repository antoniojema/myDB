#include <iostream>
#include <WinSock2.h>
#include "common/TCPSocket.h"
#include "Database.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT "80"


void communicate(Socket::Client& client_socket) {
    std::cout << "-- Reading from client --\n";

    while(true) {
        auto line = client_socket.recv_until('\n');
        if (line.has_value()) {
            if (line->length() == 0) {
                break;
            }
            std::cout << "Line: " << *line;

            auto query = Database::parse_query(*line);
            if (!query.has_value()) {
                std::cout << "Bad query.\n";
                client_socket.send_data(std::string("MyDB://Bad query - ") + *line);
            }
            else {
                std::cout << "Good query.\n";
                client_socket.send_data(std::string("MyDB://OK - ") + *line);
            }
        }
        else {
            std::cout << "No line.\n";
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

    // TCP Socket
    Socket::Server tcp_socket;
    tcp_socket.bind_and_listen(PORT);
    std::cout << "-- Listening --" << std::endl;

    // Get client
    while(true) {
        Socket::Client client_socket = tcp_socket.get_client();
        communicate(client_socket);
    }

    // Done
    tcp_socket.clear();
    WSACleanup();
    std::cout << "Done!\n";
}
