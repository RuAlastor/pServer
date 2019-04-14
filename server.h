#ifndef SERVER_H
#define SERVER_H

#ifndef HEADERS_H
    #include "headers.h"
#endif

/* SLAVE SOCKET */
class SlaveSocket {
private:
    ev::io slave_watcher;

    static unsigned int current_ID;
    const unsigned int personal_ID;
    type_socket slave_socket;
    char buffer[1024];

    void read_cb( ev::io& _watcher, int _revents );

public:
    SlaveSocket(type_socket _fd);

    virtual ~SlaveSocket();
};

/* MASTER SOCKET */
class MasterSocket {
private:
    ev::io master_watcher;
    ev::sig master_signal_watcher;

    type_socket master_socket;
    sockaddr_in* socket_info;


    void accept_cb( ev::io& _watcher, int _revents );
    static void signal_cb(ev::sig& _signal, int _revents);

public:

    MasterSocket();

    void Bind( int& _port );
    void SetToListen();
    void SetEvent();

    virtual ~MasterSocket();
};

/* GENERAL FUNCTION */
int set_nonblock( type_socket& _fd );

#endif // SERVER_H
