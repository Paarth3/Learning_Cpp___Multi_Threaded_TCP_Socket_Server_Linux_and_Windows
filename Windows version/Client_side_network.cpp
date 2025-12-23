#include <iostream>
#include <cstring>
#include <cstdint>
#include <winsock2.h>
#include <ws2tcpip.h>

struct Player{
    char player_id;
    int x_pos;
};

int main(){

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0){
        std::cerr << "Failed to start! \n";
        return 1;
    }

    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET){
        std::cerr << "Failed to create client socket! \n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0){
        std::cerr << "Invalid address / Address not supported! \n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0){
        std::cerr << "Failed to connect! \n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected to the server! \n";

    Player myPlayer;
    myPlayer.player_id = 'P';
    myPlayer.x_pos = 900;

    char buffer[5];
    buffer[0] = myPlayer.player_id;

    uint32_t temp = htonl(myPlayer.x_pos);
    std::memcpy(&buffer[1], &temp, sizeof(uint32_t));
    
    send(client_socket, buffer, 5, 0);
    std::cout << "******************\n";
    std::cout << "Data sent: \n - Team: " << myPlayer.player_id << " \n - X: " << myPlayer.x_pos << '\n';
    std::cout << "******************\n";

    closesocket(client_socket);
    WSACleanup();

    return 0;
}