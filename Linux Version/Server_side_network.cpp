#include <iostream>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void handle_client(int client_socket){
    int BYTES_NEDEED = 5;
    int received_bytes = 0;
    char buffer[BYTES_NEDEED];

    while (received_bytes < BYTES_NEDEED){
        int bytes = read(client_socket, buffer + received_bytes, BYTES_NEDEED - received_bytes);

        if (bytes <= 0){
            close(client_socket);
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

    close(client_socket);
}

int main(){

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1){
        std::cerr << "Failed to create the Socket! \n";
        return 1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr*)&address, sizeof(address)) < 0){
        std::cerr << "Failed to Bind! \n";
        return 1;
    }

    if (listen(server_socket, 3) < 0){
        std::cerr << "Failed to Listen! \n";
        return 1;
    }

    std::cout << "Server listening to port 8080... \n";

    while(true){
        socklen_t addrlen = sizeof(address);
        int new_socket = accept(server_socket, (struct sockaddr*)&address, &addrlen);

        if (new_socket < 0){
            std::cerr << "Accept failed \n";
            continue;
        }

        std::thread t(handle_client, new_socket);
        t.detach();
    }

    close(server_socket);

    return 0;
}
