//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_IMPL_HPP
#define CONCURRENT_SV_CLIENT_IMPL_HPP


/// Command map used to quickly find name -> command_nr association
const std :: map < std :: string, int > Client :: command_map = {
        { "signal",              __SIGNAL   },
        { "get-gas-price",       __GET_GP   },
        { "enable-sport-news",   __ENABLE_SPORTS },
        { "enable-weather-news", __ENABLE_WEATHER },
        { "quit",                __EXIT },
        { "n",                   __EVENT_MISSING }

};

bool Client :: serverInformed = false;
std :: mutex Client :: conditionMutex;
std :: condition_variable Client :: conditionVariable;

/**
 * Client constructor
 */
Client :: Client () {

    this->_server_info.sin_family = AF_INET;
    this->_server_info.sin_port = htons ( __PORT );
    this->_server_info.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
}

/**
 * Connection initializer
 * @return bool = function success
 */
auto Client :: initialize_connection () -> bool {

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


auto Client :: initialize_movementControllingThread () -> bool {

    return launch_new_thread ( & _movement_controlling_thread, & _movement_main, ( void * ) ( & this->_server_fd ) );
}


auto Client :: wait_position_initialization () -> void {

    std :: unique_lock lk ( conditionMutex );
    conditionVariable.wait ( lk, []{ return serverInformed; } );
}


/**
 * Initializing thread that handles reads from server => output to console
 * @return bool = function success
 */
auto Client :: initialize_consoleOutputThread () -> bool {

    return launch_new_thread ( & _writer_thread, & _console_output_main, ( void * ) ( & this->_server_fd ) );
}


/**
 * Initializing threads that send requests periodically
 * @return bool = function success
 */
auto Client :: initialize_pingingThreads () -> bool {

    bool returnResult = launch_new_thread (  & _pinging_thread, & _pinging_main, ( void * ) ( & this->_server_fd ) );
    sleep ( 1 );
    return returnResult;
}


/**
 * Main function of client; handles console reading => sending request to server
 */
auto Client :: client_main () -> void {

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

        bool hasParams = split_param ( buffer, p_aux );

        int request_code = command_map.find( buffer )->second;

        if ( request_code == __SIGNAL && ! hasParams ) {
            request_code = __NO_PARAM_REQUEST;
        }

        if ( request_code != __SIGNAL && hasParams ) {
            request_code = __BAD_REQUEST;
            p_aux = nullptr;
        }

        if ( -1 == this->_server_fd.write( & request_code, sizeof ( int ), p_aux, __STANDARD_BUFFER_SIZE ) ) {
            if ( errno == EPIPE ) {
                perror ( "Fatal error: Sending request to server problem" );
                exit ( EXIT_FAILURE );
            }
        }

        if ( request_code == __EXIT ) {
            pthread_cancel ( this->_writer_thread );
            break;
        }
    }

    pthread_join ( this->_writer_thread, nullptr );
    pthread_join ( this->_pinging_thread, nullptr );
}

#endif //CONCURRENT_SV_CLIENT_IMPL_HPP
