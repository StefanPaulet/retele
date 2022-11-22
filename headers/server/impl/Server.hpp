//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_SERVER_IMPL_HPP
#define CONCURRENT_SV_SERVER_IMPL_HPP

Server * Server :: _instance = nullptr;

Server :: Server () {

    _server_info.sin_family = AF_INET;
    _server_info.sin_addr.s_addr = htonl ( INADDR_ANY );
    _server_info.sin_port = htons ( __PORT );

    _p_thread_list = new std :: list < Thread * > ();
    _p_user_map    = new std :: map < int, pthread_t > ();
}


auto Server :: getInstance () -> Server * {

    if ( _instance == nullptr ) {
        _instance = new Server;
    }
    return _instance;
}


auto Server :: initialize_server () -> bool {

    if ( -1 == ( this->_socked_fd = socket ( AF_INET, SOCK_STREAM, 0 ) ) ) {
        perror ( "Socket creation error" );
        return false;
    }

    int optionToggle = 1;
    if ( -1 == setsockopt ( this->_socked_fd, SOL_SOCKET, SO_REUSEADDR, &optionToggle, sizeof ( int ) ) ) {
        perror ( "Socket option error" );
        return false;
    }

    if ( -1 == bind ( this->_socked_fd, ( sockaddr * ) & this->_server_info, sizeof ( sockaddr ) ) ) {
        perror ( "Error at bind" );
        return false;
    }

    if ( -1 == listen ( this->_socked_fd, __BACKLOG_SIZE ) ) {
        perror ( "Error at listen" );
        return false;
    }

    return true;
}


auto Server :: get_client () const -> int {

    sockaddr_in client_address;
    socklen_t  client_address_size = sizeof ( client_address );
    int client_fd;

    if ( -1 == ( client_fd = accept ( this->_socked_fd, ( sockaddr * ) & client_address, & client_address_size ) ) ) {
        perror ( "Error at accept" );
        return -1;
    }

    return client_fd;
}


auto Server :: create_thread ( int * clientFd ) -> Thread * {

    auto newThread = new Thread ( & this->_threadId [ this->_threadCount ++ ], clientFd );
    _p_thread_list->push_back ( newThread );
}

#endif //CONCURRENT_SV_SERVER_IMPL_HPP
