#include "errors.h"

Error_types::Init_error::Init_error() : error_message("Initialization error! Socket() failed.") {}
const char* Error_types::Init_error::what() {
    return error_message.c_str();
}
void Error_types::Init_error::cleanup() {
    std::cout << "Nothing to close\n";
}


Error_types::Bind_error::Bind_error(type_socket* input_socket) : error_message("Binding error! Bind() failed.") {
    socket_to_close = input_socket;
}
const char *Error_types::Bind_error::what() {
    return error_message.c_str();
}
void Error_types::Bind_error::cleanup() {
    close(*socket_to_close);
    std::cout << "Listening socket has been closed.\n";
}


Error_types::Listen_error::Listen_error(type_socket *input_socket) : error_message("Listening error! Listen() failed.") {
    socket_to_close = input_socket;
}
const char* Error_types::Listen_error::what() {
    return error_message.c_str();
}
void Error_types::Listen_error::cleanup() {
    close(*socket_to_close);
    std::cout << "Listening socket has been closed.\n";
}
