#include <iostream>
#include <cstring>
#include <cstdint>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

struct Player{
    char player_id;
    int x_pos;
};

int main(){

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1){
        std::cerr << "Failed to create client socket! \n";
        return 1;
    }

    sockaddr_in serv_address;
    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_address.sin_addr) <= 0){
        std::cerr << "Invalid address / Address not supported! \n";
        close(client_socket);
        return 1;
    }

    if (connect(client_socket, (struct sockaddr*)&serv_address, sizeof(serv_address)) < 0){
        std::cerr << "Failed to connect! \n";
        close(client_socket);
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

    close(client_socket);

    return 0;
}