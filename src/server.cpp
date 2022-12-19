//
// Created by stefan on 18.11.2022.

#include <CommonLibs>
#include <server/Server>

int main () {

    auto server = Server :: getInstance();

    if ( ! server->initializeServer () ) {
        perror ( "Failed server initialization" );
        exit ( EXIT_FAILURE );
    }

    while ( true ) {

        int * newClient = new int;
        * newClient = server->getClient();
        if ( -1 == * newClient ) {
            continue;
        }


        if ( ! server->createThread ( newClient ) ) {
            perror ( "Internal error, client couldn't be served" );
            continue;
        }
    }



}
//
