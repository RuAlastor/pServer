#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

typedef int type_socket;

int main() {
    // Socket init
    type_socket listening_socket = socket(AF_INET, SOCK_STREAM, 0);
    // Setting socket info
    struct sockaddr_in* socket_info = new sockaddr_in;
    socket_info->sin_family = AF_INET;
    socket_info->sin_port = htons(14888);
    socket_info->sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
    // Binding listening socket
    bind(listening_socket, (struct sockaddr *)socket_info, sizeof(socket_info));
    // Setting listening socket to listening
    listen(listening_socket, SOMAXCONN);


    return 0;
}
