//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_IMPL_HPP
#define CONCURRENT_SV_CLIENT_IMPL_HPP


const std :: map < std :: string, CommandIDs > Client :: commandMap = {
        { "signal",              __SIGNAL_REQUEST   },
        { "get-gas-stations",    __GET_GAS_STATIONS   },
        { "enable-sport-news",   __ENABLE_SPORTS },
        { "disable-sport-news",  __DISABLE_SPORTS },
        { "get-weather-news",    __GET_WEATHER },
        { "quit",                __EXIT },
        { "remove",              __EVENT_MISSING },
        { "clear",               __CLEAR_SCREEN }

};

bool Client :: serverInformed = false;

std :: mutex Client :: conditionMutex;

std :: condition_variable Client :: conditionVariable;


Client :: Client () {

    this->_server_info.sin_family = AF_INET;
    this->_server_info.sin_port = htons ( __PORT );
    this->_server_info.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
}


auto Client :: initializeConnection () -> bool {

    if ( -1 == ( this->_server_fd = socket ( AF_INET, SOCK_STREAM, 0 ) ) ) {
        perror ( "Error at socket" );
        return false;
    }

    if ( -1 == connect ( ( int ) this->_server_fd, ( sockaddr * ) ( & this->_server_info ), sizeof ( this->_server_info ) ) ) {
        perror ( "Error at connect" );
        return false;
    }

    return true;

}


auto Client :: initializeMovementControllingThread () -> bool {

    return launch_new_thread ( & _movement_controlling_thread, & _movement_main, ( void * ) ( & this->_server_fd ) );
}


auto Client :: waitPositionInitialization () -> void {

    std :: unique_lock lk ( conditionMutex );
    conditionVariable.wait ( lk, []{ return serverInformed; } );
}


auto Client :: initializeConsoleOutputThread () -> bool {

    return launch_new_thread ( & _writer_thread, & _console_output_main, ( void * ) ( & this->_server_fd ) );
}


auto Client :: initializePingingThreads () -> bool {

    bool returnResult = launch_new_thread (  & _pinging_thread, & _pinging_main, ( void * ) ( & this->_server_fd ) );
    sleep ( 1 );
    return returnResult;
}


auto Client :: clientMain () -> void {

    char buffer[__STANDARD_BUFFER_SIZE];
    char * p_aux;

    auto split_param = [] ( char * initialBuffer, char * & parameters ) -> bool {

        parameters = strchr ( initialBuffer, ' ' );
        if ( parameters == nullptr ) {
            return false;
        }

        * parameters = 0;
        parameters ++;
        return true;
    };

    while ( true ) {

        fgets ( buffer, __STANDARD_BUFFER_SIZE, stdin );

        buffer [ strlen ( buffer ) - 1 ] = 0;

        printf ( "\033[1F\033[2K" );

        bool hasParams = split_param ( buffer, p_aux );

        CommandIDs request_code = commandMap.find( buffer )->second;

        if ( request_code == __SIGNAL_REQUEST && ! hasParams ) {
            request_code = __NO_PARAM_REQUEST;
        } else if ( request_code != __SIGNAL_REQUEST && hasParams ) {
            request_code = __BAD_REQUEST;
            p_aux = nullptr;
        }

        if ( -1 == this->_server_fd.write( & request_code, sizeof ( request_code ), p_aux, __STANDARD_BUFFER_SIZE ) ) {
            if ( errno == EPIPE ) {
                perror ( "Fatal error: Sending request to server problem" );
                exit ( EXIT_FAILURE );
            }
        }

        if ( request_code == __EXIT ) {
            pthread_cancel ( this->_writer_thread );
            pthread_cancel ( this->_pinging_thread );
            pthread_cancel ( this->_movement_controlling_thread );
            break;
        }
    }

    pthread_join ( this->_writer_thread, nullptr );
    pthread_join ( this->_pinging_thread, nullptr );
    pthread_join ( this->_movement_controlling_thread, nullptr );
}

#endif //CONCURRENT_SV_CLIENT_IMPL_HPP
