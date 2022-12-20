//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include <map>
#include <string>
#include <mutex>
#include <condition_variable>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "concurrentDescriptor/AtomicSocket.hpp"
#include "../common/thread/ThreadCommons.hpp"

class Client {

private:
    static const std :: map < std :: string, CommandIDs > commandMap;

    static std :: mutex conditionMutex;

    static std :: condition_variable conditionVariable;

    static bool serverInformed;

    sockaddr_in _server_info { };

    AtomicSocket _server_fd { 0 };

    pthread_t _writer_thread { };

    pthread_t _pinging_thread { };

    pthread_t _movement_controlling_thread { };

    friend auto _movement_main ( void * param ) -> void *;   /* NOLINT(bugprone-reserved-identifier) */

public:
    Client();

    [[nodiscard]] auto initializeConnection () -> bool;

    [[nodiscard]] auto initializeMovementControllingThread () -> bool;

    static auto waitPositionInitialization () -> void;

    [[nodiscard]] auto initializePingingThreads () -> bool;

    [[nodiscard]] auto initializeConsoleOutputThread () -> bool;

    auto clientMain () -> void;
};


#include "thread/ClientThread.hpp"      /* NOLINT(llvm-include-order) */
#include "impl/Client.hpp"

#endif //CONCURRENT_SV_CLIENT_HPP
