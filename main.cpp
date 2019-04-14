

#include "headers.h"
#include "server.h"


unsigned int SlaveSocket::current_ID = 0;

int main( /* int argc, char* argv[] */ ) {

    /* Check args for port here */
    int default_port = 12345;

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




