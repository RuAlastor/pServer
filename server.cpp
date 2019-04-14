

#include "server.h"

/* SLAVE SOCKET */
/* Private */
void SlaveSocket::read_cb(ev::io &_watcher, int _revents) {
    if (EV_ERROR & _revents) {
        perror("Invalid event!");
        return;
    }

    int amount_read = recv( _watcher.fd, buffer, sizeof(buffer), 0 );
    if ( amount_read < 0 ) {
        perror("Read error!");
        return;
    }
    if ( amount_read == 0 ) {
        delete this;
    }
    else {
        send( _watcher.fd, buffer, amount_read, MSG_NOSIGNAL);
    }
}

/* Public */
SlaveSocket::SlaveSocket(type_socket _fd) : personal_ID(current_ID++), slave_socket(_fd) {
    set_nonblock(_fd);
    std::cout << "Got a connection!\n";

    slave_watcher.set<SlaveSocket, &SlaveSocket::read_cb>(this);
    slave_watcher.start(_fd, ev::READ);
}



/* MASTER SOCKET */
/* Private */
void MasterSocket::accept_cb(ev::io &_watcher, int _revents) {
    if (EV_ERROR & _revents) {
        perror("Invalid event!");
        return;
    }

    sockaddr_in client_socket_info;
    socklen_t client_socket_info_size;
    int slave_socket = accept( _watcher.fd, reinterpret_cast<sockaddr*>(&client_socket_info), &client_socket_info_size );
    if (slave_socket < 0) {
        perror("Accept error!");
        return;
    }

    SlaveSocket *client = new SlaveSocket(slave_socket);
}

/* Public */
MasterSocket::MasterSocket() {
    master_socket = socket(AF_INET, SOCK_STREAM, 0);
                        /* IPv4     TCP          IPPROTO_TCP */
    if ( master_socket == -1 ) {
        perror("Initialization error!");
    }
    std::cout << "Listening socket has been initialized!\n";
}

void MasterSocket::Bind(int& _port) {
    socket_info = new sockaddr_in;
    memset( &socket_info->sin_zero, 0, sizeof(socket_info->sin_zero) );
    socket_info->sin_family = AF_INET;                              /* AF_INET / 2 / IPv4 */
    socket_info->sin_port = htons(_port);                           /* Default port - 12345 / Set by user */
    socket_info->sin_addr.s_addr = htonl(INADDR_LOOPBACK);          /* Default IP - 127.0.0.1 */

    if ( bind( master_socket, reinterpret_cast<sockaddr*>(socket_info), sizeof(*socket_info) ) == -1 ) {
        delete socket_info;
        perror("Bind error!");
    }

    set_nonblock(master_socket);
    delete socket_info;
    std::cout << "Listening socket has been binded!\n";
}

void MasterSocket::SetToListen() {
    if ( listen( master_socket, SOMAXCONN ) == -1 ) {
        perror("Error on setting to listen step!");
    }
    std::cout << "Waiting for connection...\n";
}

void MasterSocket::SetEvent() {
    master_watcher.set<MasterSocket, &MasterSocket::accept_cb>(this);
    master_watcher.start(master_socket, ev::READ);

    /* to be continue... */
}

MasterSocket::~MasterSocket() {
    shutdown(master_socket, SHUT_RDWR);
    close(master_socket);
    std::cout << "Listening socket has been closed!\n";
}

/* GENERAL FUNCTION */
int set_nonblock(int& fd) {
    int flags;
#ifdef O_NONBLOCK
    if ( (flags = fcntl(fd, F_GETFL, 0)) == -1 ) {
        flags = 0;
    }
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
}
