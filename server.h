#ifndef SERVER_H
#define SERVER_H

#include "logsqueue.h"

/* SLAVE SOCKET */
class SlaveSocket {
private:
    ev::io slave_watcher;

    static unsigned int current_ID;
    const unsigned int personal_ID;
    type_socket slave_socket;

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

    pthread_t loggers[4];
    std::string fileName[4];

    void accept_cb( ev::io& _watcher, int _revents );
    static void signal_cb(ev::sig& _signal, int _revents);

    friend void* getLogs(void *_rawData);

public:

    MasterSocket();

    void Bind( int& _port );
    void SetToListen();
    void SetEvent();
    void SetLoggers();

    virtual ~MasterSocket();
};

/* GENERAL FUNCTION */
extern LogsQueue logs_queue;

int set_nonblock( type_socket& _fd );
void* getLogs(void* _rawData);

#endif // SERVER_H
