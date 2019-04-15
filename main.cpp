

#include "headers.h"
#include "server.h"


unsigned int SlaveSocket::current_ID = 0;

int main( int argc, char* argv[] ) {

    /* Check args for port here */
    int default_port = 12345;
    if ( argc > 1 ) {
        default_port = atoi(argv[1]);
    }
    else {
        std::cout << "Default port is 12345.\nNext time use: " << argv[0] << "<port>";
    }

    /* Socket initialization */
    MasterSocket server;

    /* Setting socket info */
    /* Binding listening socket */
    server.Bind(default_port);

    /* Setting listening socket to listening */
    server.SetToListen();

    /* Create threads for logs here */


    /* Accept connections here */
    ev::default_loop loop;
    server.SetEvent();
    loop.run();
}




