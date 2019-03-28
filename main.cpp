#include <iostream>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>

typedef int type_socket;


class Error: public std::exception {
public:
    virtual const char* what() = 0;

    virtual void cleanup() = 0;
};
namespace Error_types {

    class Init_error: public Error {
    public:
        Init_error() : error_message("Initialization error! Socket() failed.") {}

        const char* what() {
            return error_message.c_str();
        }

        void cleanup() {
            std::cout << "Nothing to close\n";
        }
    private:
        std::string error_message;
    };

    class Bind_error: public Error {
    public:
        Bind_error(type_socket* input_socket, sockaddr_in* input_sockaddr): error_message("Binding error! Bind() failed.") {
            socket_to_close = input_socket;
            sockaddr_to_clear = input_sockaddr;
        }

        const char* what() {
            return error_message.c_str();
        }

        void cleanup() {
            delete sockaddr_to_clear;
            close(*socket_to_close);
            std::cout << "Listening socket has been closed.\n";
        }
    private:
        std::string error_message;
        type_socket* socket_to_close;
        sockaddr_in* sockaddr_to_clear;
    };

    class Listen_error: public Error {
    public:
        Listen_error(type_socket* input_socket): error_message("Listening error! Listen() failed.") {
            socket_to_close = input_socket;
        }

        const char* what() {
            return error_message.c_str();
        }

        void cleanup() {
            close(*socket_to_close);
            std::cout << "Listening socket has been closed.\n";
        }
    private:
        std::string error_message;
        type_socket* socket_to_close;
    };

}

int main(int argc, char* argv[]) {
    try {
        // Socket initialization
        type_socket listening_socket = socket(AF_INET, SOCK_STREAM, 0);
                                            // IPv4     TCP     IPPROTO_TCP
        if ( listening_socket == -1 ) {
            throw Error_types::Init_error();
        }
        std::cout << "Listening socket has been opened!\n";

        // Setting socket info
        struct sockaddr_in* socket_info = new sockaddr_in;
        socket_info->sin_family = AF_INET;
        socket_info->sin_port = htons(14888); // Change later
        socket_info->sin_addr.s_addr = htonl(INADDR_LOOPBACK); // 127.0.0.1
        // Binding listening socket
        if ( bind(listening_socket, (struct sockaddr *)socket_info, sizeof(socket_info)) != 0 ) {
            throw Error_types::Bind_error(&listening_socket, socket_info);
        }
        std::cout << "Listen socket has been binded!\n";
        delete socket_info;

        // Setting listening socket to listening
        if ( listen(listening_socket, SOMAXCONN) == -1 ) {
            throw Error_types::Listen_error(&listening_socket);
        }


        return 0;
    }

    catch (Error& ERROR) {
        std::cout << ERROR.what() << "\n";
        ERROR.cleanup();
    }
}

