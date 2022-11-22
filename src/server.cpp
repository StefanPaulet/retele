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

    while ( true ) {

        int newClient = server->get_client();
        if ( -1 == newClient ) {
            continue;
        }

        break;
    }
}
//
