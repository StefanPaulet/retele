//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_IMPL_HPP
#define CONCURRENT_SV_CLIENT_IMPL_HPP


const std :: map < std :: string, int > Client :: _command_map = {
        { "signal",              __SIGNAL   },
        { "get-gas-price",       __GET_GP   },
        { "enable-sport-news",   __ENABLE_S },
        { "enable-weather-news", __ENABLE_W },
        { "quit",                __EXIT }
};

Client :: Client () {

    this->_server_info.sin_family = AF_INET;
    this->_server_info.sin_port = htons ( __PORT );
    this->_server_info.sin_addr.s_addr = inet_addr ( "127.0.0.1" );
}


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


auto Client :: initialize_writerThread () -> bool {

    return launch_new_thread ( & _writerThread, & _client_reading_endpoint, & _server_fd );
}

auto Client :: initialize_pingingThreads () -> bool {

    bool returnResult = true;

    int p_ping_param[5][3];

    p_ping_param [ 0 ][ 2 ] = 1;
    p_ping_param [ 1 ][ 2 ] = 10;
    p_ping_param [ 2 ][ 2 ] = 10;
    p_ping_param [ 3 ][ 2 ] = 60;
    p_ping_param [ 4 ][ 2 ] = 1;

    for ( int i = 0; i < 5; ++ i ) {
        p_ping_param [ i ][ 0 ] = this->_server_fd;
        p_ping_param [ i ][ 1 ] = i + __TIMED_EVENTS_UPDATE;

        returnResult = returnResult &&
                launch_new_thread (  & _pinging_thread[ i ], & _client_pinging, p_ping_param[ i ] );
    }

    sleep ( 1 );

    return returnResult;
}


auto Client :: client_main () const -> void {

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

        if ( request_code / 100 != 0 && ! hasParams ) {
            request_code = __BAD_REQUEST;
        }

        if ( -1 == write ( this->_server_fd, & request_code, sizeof ( int ) ) ) {
            perror ( "Error at writing" );
            exit ( EXIT_FAILURE );
        }

        if ( request_code / 100 != 0 && hasParams ) {
            write ( this->_server_fd, p_aux, __STANDARD_BUFFER_SIZE );
        }

        if ( request_code == __EXIT ) {
            pthread_cancel ( this->_writerThread );
            break;
        }
    }

    pthread_join ( this->_writerThread, nullptr );
    for ( auto i : this->_pinging_thread ) {
        pthread_join ( i, nullptr );
    }
}
#endif //CONCURRENT_SV_CLIENT_IMPL_HPP
