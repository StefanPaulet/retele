//
// Created by stefan on 18.11.2022.
//


#include <CommonLibs>
#include <client/Client>

int main () {

    auto client = new Client;

    if ( ! client->initialize_connection() ) {
        perror ( "Fatal error" );
        exit ( EXIT_FAILURE );
    }

    printf ( "Connected to server\n" );

    if ( ! client->initialize_consoleOutputThread() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    if ( ! client->initialize_pingingThreads() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    client->client_main();
}