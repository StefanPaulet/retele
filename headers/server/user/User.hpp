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

private:    /* NOLINT(readability-redundant-access-specifiers) */
    static Graph * pGraph;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    static std :: array < std :: string, 6 > const signalNamesArray;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    friend auto _queue_supervisor_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

private:    /* NOLINT(readability-redundant-access-specifiers) */
     AtomicQueue :: QueueNode _pLocalEventNode { pEventQueue->front() };

private:    /* NOLINT(readability-redundant-access-specifiers) */
    int _client_fd;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    Vehicle _userPosition { 30, 0 };

private:    /* NOLINT(readability-redundant-access-specifiers) */
    uint8 _waiting_response { 0 };

public:     /* NOLINT(readability-redundant-access-specifiers) */
    explicit User ( int clientFd ) :
        _client_fd ( clientFd ) {

    };

private:    /* NOLINT(readability-redundant-access-specifiers) */
    auto send_msg ( std :: string const & message ) const -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto handle_request () -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto handle_signal () -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto handle_event_update () -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto handle_speed_limit_update () -> void;

public:     /* NOLINT(readability-redundant-access-specifiers) */
    auto handle_event_removal () -> void;

private:    /* NOLINT(readability-redundant-access-specifiers) */
    auto remove_street_event ( uint8 eventType ) const -> void;
};

#include "../../common/vehicle/impl/Vehicle.hpp"        /* NOLINT(llvm-include-order) */
#include "../../common/graph/impl/Graph.hpp"

#endif //CONCURRENT_SV_USER_HPP
