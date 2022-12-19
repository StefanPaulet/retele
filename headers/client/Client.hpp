//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include "concurrentDescriptor/AtomicSocket.hpp"

class Client {

private:
    static const std :: map < std :: string, sint16 > command_map;

private:
    static std :: mutex conditionMutex;

private:
    static std :: condition_variable conditionVariable;

private:
    static bool serverInformed;

    friend auto _movement_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

private:
    sockaddr_in _server_info { };

private:
    AtomicSocket _server_fd { 0 };

private:
    pthread_t _writer_thread { };

private:
    pthread_t _pinging_thread { };

private:
    pthread_t _movement_controlling_thread { };

public:
    Client();

public:
    [[nodiscard]] auto initialize_connection () -> bool;

public:
    [[nodiscard]] auto initialize_movementControllingThread () -> bool;

public:
    static auto wait_position_initialization () -> void;

public:
    [[nodiscard]] auto initialize_pingingThreads () -> bool;

public:
    [[nodiscard]] auto initialize_consoleOutputThread () -> bool;

public:
    auto client_main () -> void;
};


#include "thread/ClientThread.hpp"      /* NOLINT(llvm-include-order) */
#include "impl/Client.hpp"

#endif //CONCURRENT_SV_CLIENT_HPP
