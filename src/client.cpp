//
// Created by stefan on 18.11.2022.
//


#include <CommonLibs>
#include <client/Client>


int main () {

    auto client = new Client;

    if ( ! client->initializeConnection() ) {
        perror ( "Fatal error" );
        exit ( EXIT_FAILURE );
    }

    printf ( "Connected to server" );
    fflush ( stdout );
    sleep ( 1 );
    printf ( "\033[2K\n\n\n\n\n\n\n\n\n\n" );

    if ( ! client->initializeMovementControllingThread() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    Client :: waitPositionInitialization();

    if ( ! client->initializeConsoleOutputThread() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    if ( ! client->initializePingingThreads() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    client->clientMain();
}