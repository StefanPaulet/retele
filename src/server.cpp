//
// Created by stefan on 18.11.2022.

#include <CommonLibs>
#include <server/Server>

int main () {

    auto server = Server :: getInstance();

    if ( ! server->initialize_server () ) {
        perror ( "Failed server initialization" );
        exit ( EXIT_FAILURE );
    }

    printf ( "Server successfully initialized\n" );
    while ( true ) {

        int * newClient = new int;
        * newClient = server->get_client();
        if ( -1 == * newClient ) {
            continue;
        }

        printf ( "Client connected\n" );

        if ( ! server->create_thread ( newClient ) ) {
            perror ( "Internal error, client couldn't be served" );
            continue;
        } else {
            printf ( "Created thread\n" );
        }
    }



}
//
