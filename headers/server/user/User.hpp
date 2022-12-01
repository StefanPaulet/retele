//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_HPP
#define CONCURRENT_SV_USER_HPP

#include <AtomicQueue>
#include <memory>

class User {

private:
    static std :: shared_ptr < AtomicQueue > pEventQueue;

private:
    std :: shared_ptr < AtomicQueue > _pLocalEventQueue;

private:
    int _client_fd;

public:
    explicit User ( int clientFd ) :
        _client_fd ( clientFd ),
        _pLocalEventQueue ( pEventQueue ) {

    };

public:
    ~User () {
        this->_pLocalEventQueue.reset ();
    }

private:
    auto send_msg ( std :: string const & message ) const -> void;

public:
    auto handle_request () -> void;

public:
    auto handle_signal () -> int;
};

#endif //CONCURRENT_SV_USER_HPP
