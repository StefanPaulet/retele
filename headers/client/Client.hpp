*//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include <list>
#include <mutex>
#include <condition_variable>
#include <map>
#include "concurrentDescriptor/ConcurrentDescriptor.hpp"

class Client {

private:
    static const std :: map < std :: string, int, std :: less <> > command_map;

private:
    static std :: mutex conditionMutex;
    static std :: condition_variable conditionVariable;
    static bool serverInformed;

    friend auto _movement_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

private:
    sockaddr_in _server_info { };

private:
    ConcurrentDescriptor _server_fd { 0 };

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
    [[nodiscard]] auto initialize_pingingThreads () -> bool;

public:
    [[nodiscard]] auto initialize_consoleOutputThread () -> bool;

public:
    [[nodiscard]] auto initialize_movementControllingThread () -> bool;

public:
    auto client_main () -> void;
};


#include "thread/ClientThread.hpp"      /* NOLINT(llvm-include-order) */
#include "impl/Client.hpp"

#endif //CONCURRENT_SV_CLIENT_HPP
