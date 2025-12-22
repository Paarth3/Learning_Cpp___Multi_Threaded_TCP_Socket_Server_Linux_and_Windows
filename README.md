# Multi-Threaded TCP Socket Server (Windows)

A low-level networking project built in C++ that demonstrates the core fundamentals of the Berkeley Socket API. This project implements a **Client-Server architecture** capable of handling multiple concurrent connections, performing manual memory serialization, and ensuring data integrity across network byte orders.

## 🚀 Project Overview

This application establishes a reliable TCP connection between a client and a server. It solves the common pitfalls of C++ networking (specifically **structure padding** and **endianness mismatches**) by manually serializing data into a raw byte stream rather than sending raw structs.

The server is **multi-threaded**, allowing it to accept and process new connections immediately without blocking existing clients.

## 🛠 Key Concepts Implemented

* **Winsock API:** Initialization and management of Windows Sockets (`WSAStartup`, `socket`, `bind`, `listen`, `accept`).
* **TCP Protocol:** Handling the stream-based nature of TCP (looping on `recv` to ensure complete message delivery).
* **Multi-Threading:** Using `std::thread` to handle multiple clients simultaneously (`detach` mode).
* **Serialization:** Manually packing data (`memcpy`) to avoid compiler-added memory padding.
* **Endianness:** Converting integers from Host Byte Order (Little Endian) to Network Byte Order (Big Endian) using `htonl()` and `ntohl()`.

## 📂 File Structure

* **`Server_side_network.cpp`**: The host application. It listens on port `8080`, spins up a new thread for every incoming connection, and deserializes the incoming data stream.
* **`Client_side_network.cpp`**: The client application. It packs a custom `Player` data structure into a byte buffer and transmits it to the server.

## 📦 The Protocol

The application uses a custom binary protocol to transmit player data. To ensure the data is compact and platform-independent, we do not send the C++ `struct` directly.

**Data Structure:**
```cpp
struct Player {
    char player_id; // 1 Byte
    int x_pos;      // 4 Bytes
};
```

**Network Packet Layout (5 Bytes Total):** Instead of the 8 bytes a compiler might generate (due to padding), we pack exactly 5 bytes:

| **Byte Index** | **Data Type** | **Content** | **Note**                |
| -------------- | ------------- | ----------- | ----------------------- |
| **0**          | `char`        | `player_id` | Sent as-is              |
| **1-4**        | `uint32_t`    | `x_pos`     | Converted to Big-Endian |

## ⚙️ How to Build & Run

### Prerequisites

- Windows OS
- A C++ Compiler (MinGW `g++` or MSVC)
- Visual Studio Code (optional)

### Compilation

Because this project uses the Windows Socket library (`Winsock2`), you must link against the `Ws2_32` library during compilation.

**Using g++ (MinGW):**

```Bash
# Compile Server
g++ Server_side_network.cpp -o server.exe -lws2_32

# Compile Client
g++ Client_side_network.cpp -o client.exe -lws2_32
```

**Using Visual Studio (MSVC):**

The code already includes `#pragma comment(lib, "Ws2_32.lib")`, so you can compile normally without adding extra linker flags.

### Running the Application

1. Start the Server:
    Open a terminal and run:
        
    ```Bash
    .\Server_side_network.exe
    ```
    
    _Output:_ `Server listening to port 8080...`
    
2. Start the Client:
    
    Open a new terminal window and run:
    
    ```Bash
    .\Client_side_network.exe
    ```
    
3. **Observation:**
    
    - The **Client** will print: `Connected to the server!` and `Data sent...`.
    - The **Server** will detect the connection, spawn a thread, and print the unpacked data: `[Thread ID] Received... Player X-position: 900`.
