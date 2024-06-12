# NetTalk-Simple-TCP-Chat

## Overview
This project is a minimalistic chat application written in C that uses TCP/IP networking to facilitate real-time communication between a server and client. The application supports both server and client functionalities, enabling two users to exchange messages over a network.

## Features
- **Server and Client Modes**: The application can be started in either server or client mode.
- **Ephemeral Ports**: When started without specifying a port, the server picks a random port in the ephemeral range.
- **Polling**: Uses the `poll` system call to handle simultaneous input from the terminal and network socket without multi-threading.
- **Timeouts**: Implements a 1-minute timeout on the poll to ensure the application remains responsive.
- **Buffer Management**: Handles up to 4096 bytes of data transmission at a time.

## Compilation and Usage

### Prerequisites
Ensure you have a C compiler (like gcc) and the standard C libraries available on your system. This application uses the `sys/socket.h`, `arpa/inet.h`, and `unistd.h` libraries, which are typically available on Unix-like systems.

### Compilation
Compile the application using the following command:
```bash
gcc nettchat.c -o nettchat
