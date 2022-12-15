//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_IMPL_HPP
#define CONCURRENT_SV_CLIENT_IMPL_HPP

/// Command map used to quickly find name -> command_nr association
const std :: map < std :: string, int > Client :: _command_map = {
        { "signal",              __SIGNAL   },
        { "get-gas-price",       __GET_GP   },
        { "enable-sport-news",   __ENABLE_SPORTS },
        { "enable-weather-news", __ENABLE_WEATHER },
        { "quit",                __EXIT },
        { "n",                   __EVENT_MISSING },

};

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

    if ( -1 == connect ( this->_server_fd, ( sockaddr * ) ( & _server_info ), sizeof ( _server_info ) ) ) {
        perror ( "Error at connect" );
        return false;
    }

    return true;

}

/**
 * Initializing thread that handles reads from server => output to console
 * @return bool = function success
 */
auto Client :: initialize_consoleOutputThread () -> bool {

    return launch_new_thread ( & _writer_thread, & _console_output_main, ( void * ) ( & _server_fd ) );
}


auto Client :: initialize_movementControllingThread () -> bool {

    return launch_new_thread ( & _movement_controlling_thread, & _movement_main, ( void * ) this );
}


/**
 * Initializing threads that send requests periodically
 * @return bool = function success
 */
auto Client :: initialize_pingingThreads () -> bool {

    bool returnResult = launch_new_thread (  & _pinging_thread, & _pinging_main, ( void * ) this );
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

        int request_code = _command_map.find( buffer )->second;

        if ( request_code == __SIGNAL && ! hasParams ) {
            request_code = __NO_PARAM_REQUEST;
        }

        if ( -1 == this->server_write ( & request_code, sizeof ( int ) ) ) {
            perror ( "Error at writing to server" );
            if ( errno == EPIPE ) {
                exit ( EXIT_FAILURE );
            }
        }

        if ( request_code == __SIGNAL && hasParams ) {
            if ( -1 == this->server_write ( p_aux, __STANDARD_BUFFER_SIZE ) ) {
                perror ( "Error at writing to server" );
                if ( errno == EPIPE ) {
                    exit ( EXIT_FAILURE );
                }
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


auto Client :: server_write (
        void * message,
        uint16 length
) -> sint64 {

    std :: lock_guard lock ( this->_socket_lock );
    return write ( this->_server_fd, message, length );
}

#endif //CONCURRENT_SV_CLIENT_IMPL_HPP
