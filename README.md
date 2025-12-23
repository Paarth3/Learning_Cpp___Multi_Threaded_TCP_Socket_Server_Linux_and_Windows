# Multi-Threaded TCP Socket Server (Cross-Platform)

A low-level networking project built in C++ that demonstrates the core fundamentals of the Berkeley Socket API. This project implements a **Client-Server architecture** capable of handling multiple concurrent connections, performing manual memory serialization, and ensuring data integrity across network byte orders on both **Windows** and **Linux**.



## 🚀 Project Overview

This application establishes a reliable TCP connection between a client and a server. It solves the common pitfalls of C++ networking (specifically **structure padding** and **endianness mismatches**) by manually serializing data into a raw byte stream rather than sending raw structs.

The server is **multi-threaded**, utilizing `std::thread` to accept and process new connections immediately without blocking existing clients.

## 🛠 Key Concepts Implemented

* **Socket APIs:**
    * **Windows:** Initialization using `Winsock2` (`WSAStartup`).
    * **Linux:** Implementation using standard POSIX (BSD) sockets.
* **TCP Protocol:** Handling the stream-based nature of TCP (looping on `recv`/`read` to ensure complete message delivery).
* **Multi-Threading:** Using C++11 `std::thread` to handle multiple clients simultaneously.
* **Serialization:** Manually packing data (`memcpy`) to avoid compiler-added memory padding.
* **Endianness:** Converting integers from Host Byte Order (Little Endian) to Network Byte Order (Big Endian) using `htonl()`/`ntohl()` (Windows) and their `<arpa/inet.h>` equivalents (Linux).

## 📂 File Structure

* **`Server_side_network.cpp` (both Linux and Windows)**: The host application. It listens on port `8080`, spins up a new thread for every incoming connection, and deserializes the incoming data stream.
* **`Client_side_network.cpp` (both Linux and Windows)**: The client application. It packs a custom `Player` data structure into a byte buffer and transmits it to the server.

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

|**Byte Index**|**Data Type**|**Content**|**Note**|
|---|---|---|---|
|**0**|`char`|`player_id`|Sent as-is|
|**1-4**|`uint32_t`|`x_pos`|Converted to Big-Endian|

## ⚙️ How to Build & Run

### Prerequisites

- **C++ Compiler:** GCC/G++ or MSVC.
- **OS:** Windows 10/11 OR Linux (Ubuntu, Debian, etc.).

---

### 1. Windows Compilation

On Windows, you must link against the `Ws2_32` library.

**Using g++ (MinGW):**

```
# Compile Server
g++ Server_side_network.cpp -o server.exe -lws2_32

# Compile Client
g++ Client_side_network.cpp -o client.exe -lws2_32
```

**Using Visual Studio (MSVC):**

The code includes `#pragma comment(lib, "Ws2_32.lib")`, so you can compile normally without manual linker flags.

---

### 2. Linux Compilation

On Linux, standard headers allow socket communication without linking specific socket libraries, but you must link the **`pthread`** library for multi-threading support.

**Using g++:**

```
# Compile Server (Ensure you use the Linux source file)
g++ Server_side_network.cpp -o server -pthread

# Compile Client
g++ Client_side_network.cpp -o client
```

---

### 3. Running the Application

**Step 1: Start the Server**

Open a terminal and run:

- **Windows:** `.\server.exe`
- **Linux:** `./server`

_Output:_ `Server listening to port 8080...`

**Step 2: Start the Client**

Open a new terminal window and run:

- **Windows:** `.\client.exe`    
- **Linux:** `./client`

**Observation:**

- The **Client** will print: `Connected to the server!` and `Data sent...`.
- The **Server** will detect the connection, spawn a thread, and print the unpacked data: `[Thread ID] Received... Player X-position: 900`.

**Linux Output terminals (Left is server; right is client)**  
![Alt Text](./images/Two_terminals_running_the_code_Linux.png)

**Windows Output terminals (Left is server; right is client)**  
![Alt Text](./images/Two_terminals_running_the_code_Windows.png)
