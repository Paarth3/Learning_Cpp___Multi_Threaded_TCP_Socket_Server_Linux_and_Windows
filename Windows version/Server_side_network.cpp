#include <iostream>
#include <cstring>
#include <thread>
#include <ws2tcpip.h>
#include <winsock2.h>

void handle_client(SOCKET client_socket){
    int BYTES_NEDEED = 5;
    int received_bytes = 0;
    char buffer[BYTES_NEDEED];

    while (received_bytes < BYTES_NEDEED){
        int bytes = recv(client_socket, buffer + received_bytes, BYTES_NEDEED - received_bytes, 0);

        if (bytes <= 0){
            closesocket(client_socket);
            return;
        }

        received_bytes += bytes;
    }

    char received_player_id = buffer[0];

    uint32_t network_received_x_pos;
    std::memcpy(&network_received_x_pos, &buffer[1], sizeof(uint32_t));

    int actual_x_pos = ntohl(network_received_x_pos);

    std::cout << "******************************* \n";
    std::cout << "[Thread " << std::this_thread::get_id() << "]. Received the following data: \n"
    << " -> Pleayer ID: " << received_player_id << '\n'
    << " -> Player X-position: " << actual_x_pos << '\n';
    std::cout << "******************************* \n";

    closesocket(client_socket);
}

int main(){

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        std::cerr << "Failed to Start! \n";
        return 1;     
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == INVALID_SOCKET){
        std::cerr << "Failed to create the Socket! \n";
        WSACleanup();
        return 1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR){
        std::cerr << "Failed to Bind! \n";
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 3) == SOCKET_ERROR){
        std::cerr << "Failed to Listen! \n";
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening to port 8080... \n";

    while(true){
        int addrlen = sizeof(address);
        SOCKET new_socket = accept(server_socket, (struct sockaddr*)&address, &addrlen);

        if (new_socket == INVALID_SOCKET){
            std::cerr << "Accept failed \n";
            continue;
        }

        std::thread t(handle_client, new_socket);
        t.detach();
    }

    closesocket(server_socket);
    WSACleanup();

    return 0;
}