
#include "errors.h"

int set_nonblock(int);

int main(int argc, char* argv[]) {
    try {
        // Socket initialization
        type_socket master_socket = socket(AF_INET, SOCK_STREAM, 0);
                                            // IPv4     TCP     IPPROTO_TCP
        if ( master_socket == -1 ) {
            throw Error_types::Init_error();
        }
        std::cout << "Listening socket has been opened!\n";

        // Setting socket info
        sockaddr_in* socket_info = new sockaddr_in;
        socket_info->sin_family = AF_INET;
        socket_info->sin_port = htons(12345); // Change later
        socket_info->sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
        // Binding listening socket
        if ( bind(master_socket, (sockaddr*)socket_info, sizeof(*socket_info)) == -1 ) {
            delete socket_info;
            throw Error_types::Bind_error(&master_socket);
        }
        std::cout << "Listening socket has been binded!\n";
        delete socket_info;

        set_nonblock(master_socket);

        // Setting listening socket to listening
        if ( listen(master_socket, SOMAXCONN) == -1 ) {
            throw Error_types::Listen_error(&master_socket);
        }

        close(master_socket);
        std::cout << "Listening socket has been closed!\n";
        return 0;
    }

    catch (Error& ERROR) {
        std::cout << ERROR.what() << "\n";
        ERROR.cleanup();
    }
}

int set_nonblock(int file_desc) {
    int flags;
#if defined(O_NONBLOCK)
    if ( (flags = fcntl(file_desc, F_GETFL, 0))) {
        flags = 0;
    }
    return fcntl(file_desc, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(file_desc, FIOBID, &flags);
#endif
}






