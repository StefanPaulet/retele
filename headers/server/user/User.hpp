//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_USER_HPP
#define CONCURRENT_SV_USER_HPP

#include <AtomicQueue>
#include <memory>
#include <array>
#include "../../common/graph/Graph.hpp"
#include "../../common/vehicle/Vehicle.hpp"

class User {

private:
    static AtomicQueue * pEventQueue;

private:
    static Graph * pGraph;

private:
    static std :: array < std :: string, 6 > const signalNamesArray;

private:
    friend auto _queue_supervisor_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

private:
     AtomicQueue :: QueueNode _pLocalEventNode;

private:
    int _client_fd;

private:
    Vehicle _userPosition { 30, 0 };

private:
    uint8 _waiting_response { 0 };

public:
    explicit User ( int clientFd ) :
        _client_fd ( clientFd ),
        _pLocalEventNode ( pEventQueue->front() ) {

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

public:
    auto handle_speed_limit_update () -> void;

public:
    auto handle_event_removal () -> void;

private:
    auto remove_street_event ( uint8 eventType ) const -> void;
};

#include "../../common/vehicle/impl/Vehicle.hpp"
#include "../../common/graph/impl/Graph.hpp"

#endif //CONCURRENT_SV_USER_HPP
