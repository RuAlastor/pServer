#ifndef ERRORS_H
#define ERRORS_H
#include "headers.h"

class Error: public std::exception {
public:
    virtual const char* what() = 0;

    virtual void cleanup() = 0;
};

namespace Error_types {

    class Init_error: public Error {
    private:
        std::string error_message;
    public:
        Init_error();

        const char* what();

        void cleanup();
    };

    class Bind_error: public Error {
    private:
        std::string error_message;
        type_socket* socket_to_close;
    public:
        Bind_error(type_socket*);

        const char* what();

        void cleanup();
    };

    class Listen_error: public Error {
    private:
        std::string error_message;
        type_socket* socket_to_close;
    public:
        Listen_error(type_socket*);

        const char* what();

        void cleanup();
    };

}

#endif // ERRORS_H
