//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_HPP
#define CONCURRENT_SV_USER_HPP

#include <AtomicQueue>
#include <memory>

class User {

private:
    static AtomicQueue * pEventQueue;

private:
     AtomicQueue :: QueueNode _pLocalEventNode;

private:
    int _client_fd;

public:
    explicit User ( int clientFd ) :
        _client_fd ( clientFd ),
        _pLocalEventNode ( pEventQueue->back() ) {

        printf ( "New thread %d acquired queue pointer with %ld owners and message %s",
            this->_client_fd,
            this->_pLocalEventNode.use_count(),
            this->_pLocalEventNode.get()->_message.c_str()
        );

    };

public:
    ~User () {
        this->_pLocalEventNode.reset ();
    }

private:
    auto send_msg ( std :: string const & message ) const -> void;

public:
    auto handle_request () -> void;

public:
    auto handle_signal () -> void;

public:
    auto handle_event_update () -> void;
};

#endif //CONCURRENT_SV_USER_HPP
