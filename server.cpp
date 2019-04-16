
#include "server.h"

/* SLAVE SOCKET */
/* Private */
void SlaveSocket::read_cb( ev::io &_watcher, int _revents ) {
    if (EV_ERROR & _revents) {
        perror( "Invalid event!" );
        return;
    }

    char* buffer = new char[1024];

    int amount_read = recv( _watcher.fd, buffer, sizeof(char) * 1024, 0 );
    /* Error */
    if ( amount_read < 0 ) {
        delete [] buffer;
        perror( "Read error!" );
        return;
    }
    /* Connection closed */
    if ( amount_read == 0 ) {
        delete [] buffer;
        delete this;
    }
    /* Got something */
    else {
        char* pageSt = buffer + 4;
        char* pageEnd = std::find( pageSt, buffer + amount_read, ' ' );
        std::string page( pageSt, pageEnd );

        // std::unique_ptr<std::string> new_job;
        // logs_queue.addJob( new_job );
        delete [] buffer;

        delete this;
    }
}
/* Public */
SlaveSocket::SlaveSocket(type_socket _fd) : personal_ID(current_ID++), slave_socket(_fd) {
    set_nonblock(_fd);
    std::cout << "Got a connection!\n";

    slave_watcher.set<SlaveSocket, &SlaveSocket::read_cb>(this);
    slave_watcher.start(_fd, ev::READ);
}

SlaveSocket::~SlaveSocket() {
    slave_watcher.stop();
    close(slave_socket);

    std::cout << --current_ID << " client(s) is(are) still connected.\n";
}

/* MASTER SOCKET */
/* Private */
void MasterSocket::accept_cb(ev::io &_watcher, int _revents) {
    if ( EV_ERROR & _revents ) {
        perror( "Invalid event!" );
        return;
    }

    sockaddr_in client_socket_info;
    socklen_t client_socket_info_size;
    int slave_socket = accept( _watcher.fd, reinterpret_cast<sockaddr*>(&client_socket_info), &client_socket_info_size );
    if (slave_socket < 0) {
        perror( "Accept error!" );
        return;
    }

    SlaveSocket* client = new SlaveSocket(slave_socket);
    client = nullptr;
}

void MasterSocket::signal_cb( ev::sig &_signal, int _revents ) {
    _signal.loop.break_loop();
}

/* Public */
MasterSocket::MasterSocket() {
    master_socket = socket(AF_INET, SOCK_STREAM, 0);
                        /* IPv4     TCP          IPPROTO_TCP */
    if ( master_socket == -1 ) {
        perror( "Initialization error!" );
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
        perror( "Bind error!" );
    }

    set_nonblock(master_socket);
    delete socket_info;
    std::cout << "Listening socket has been binded!\n";
}

void MasterSocket::SetToListen() {
    if ( listen( master_socket, SOMAXCONN ) == -1 ) {
        perror( "Error on setting to listen step!" );
    }
    std::cout << "Waiting for connection...\n";
}

void MasterSocket::SetEvent() {
    master_watcher.set<MasterSocket, &MasterSocket::accept_cb>(this);
    master_watcher.start(master_socket, ev::READ);

    master_signal_watcher.set<&MasterSocket::signal_cb>();
    master_signal_watcher.start(SIGINT);
}

void MasterSocket::SetLoggers() {
    for (int i = 0; i < 4; i++) {
        fileName[i] = "log" + std::to_string(i) + ".txt";
        pthread_create(&loggers[i], NULL, getLogs, reinterpret_cast<void*>(&fileName[i]) );
        pthread_detach(loggers[i]);
    }
}

MasterSocket::~MasterSocket() {
    shutdown(master_socket, SHUT_RDWR);
    close(master_socket);
    std::cout << "Listening socket has been closed!\n";
}

/* GENERAL FUNCTION */
int set_nonblock(type_socket& fd) {
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


void* getLogs(void *_rawData) {
    std::string fileName = *reinterpret_cast<std::string*>(_rawData);
    // std::cout << "A new thread started\n";
    // std::cout << fileName << '\n';

    std::ofstream f_out(fileName);

    if ( !f_out.is_open() ) {
        perror( "Failed to open file" );
        return NULL;
    }
    std::cout << "1";

    std::unique_ptr<std::string> job;
    while ( job = logs_queue.getJob() ) {
        if ( job ) {
            f_out << *job << '\n';
        }
    }


    return NULL;
}
