# Socket-Programming

**Socket:** An interface between an application process and transport layer for Inter-Process communication. The application process can send/receive messages to/from another application process (local or remote) via a socket.

This repository contains programs for socket programming using C and C++.

## Programs
- [File Transfer](https://github.com/arefa29/Socket-Programming/tree/main/FileTransfer)
- Multi client chat server

## Running

1. Clone the repository

```
git clone https://github.com/arefa29/Socket-Programming.git
```

2. Open the program folder which you want to run in two terminals for client and server
3. On server terminal run

```
g++ server.cpp -o server
./server
```

4. On client terminal run

```
g++ client.cpp -o client
./client
```
