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

    printf ( "Connected to server\n" );

    if ( ! client->initializeMovementControllingThread() ) {
        perror ( "Thread creation error" );
        exit ( EXIT_FAILURE );
    }

    Client :: waitPositionInitialization();

    printf ( "Position has been sent to server\n" );

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