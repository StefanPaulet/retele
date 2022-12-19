//
// Created by stefan on 19.11.2022.
//

#ifndef CONCURRENT_SV_SERVER_IMPL_HPP
#define CONCURRENT_SV_SERVER_IMPL_HPP

Server * Server :: _instance = nullptr;


Server :: Server () {

    this->_server_info.sin_family = AF_INET;
    this->_server_info.sin_addr.s_addr = htonl ( INADDR_ANY );
    this->_server_info.sin_port = htons ( __PORT );
}


auto Server :: getInstance () -> Server * {

    if ( _instance == nullptr ) {
        _instance = new Server;
    }
    return _instance;
}


auto Server :: initializeServer () -> bool {

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

    return  launch_new_thread ( & this->_queue_supervisor_id, & _queue_supervisor_main, nullptr );
}


auto Server :: getClient () const -> int {

    sockaddr_in client_address { };
    socklen_t  client_address_size = sizeof ( client_address );
    int client_fd;

    if ( -1 == ( client_fd = accept ( this->_socked_fd, ( sockaddr * ) & client_address, & client_address_size ) ) ) {
        perror ( "Error at accept" );
        return -1;
    }

    return client_fd;
}


auto Server :: createThread ( int * clientFd ) -> bool {


    pthread_t newThread;

    bool returnStatus = launch_new_thread ( & newThread, & _server_main, clientFd );
    if ( ! returnStatus ) {
        Server :: serverError( * clientFd, "Internal server error, try again later" );

    }

    std :: lock_guard lock ( this->_thread_list_lock );
    this->_thread_list.push_back ( newThread );

    return returnStatus;
}


auto Server :: serverError ( int clientFd, std :: string const & error ) -> void {

    auto bufferSize = error.size();
    write ( clientFd, & bufferSize, sizeof ( bufferSize ) );
    write ( clientFd, error.c_str(), __STANDARD_BUFFER_SIZE );
    close ( clientFd );
}

auto Server :: disconnectClient ( pthread_t threadId ) -> void {

    std :: lock_guard lock ( this->_thread_list_lock );
    this->_thread_list.remove ( threadId );
}

#endif //CONCURRENT_SV_SERVER_IMPL_HPP
