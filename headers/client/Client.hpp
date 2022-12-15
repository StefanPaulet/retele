//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include <list>
#include <map>
#include <mutex>
#include "concurrentDescriptor/ConcurrentDescriptor.hpp"

class Client {

private:
    struct PingingThreadParameter {
        int requestNr { 0 };
        int frequency { 0 };
        Client * client { nullptr };
    };

friend auto _pinging_main ( void * param ) -> void *; /* NOLINT(bugprone-reserved-identifier) */

private:
    sockaddr_in _server_info { };

private:
    ConcurrentDescriptor ;

private:
    pthread_t _writer_thread { };

private:
    pthread_t _pinging_thread { };

private:
    pthread_t _movement_controlling_thread { };

private:
    static const std :: map < std :: string, int > _command_map;

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

public:
    auto server_write (
            void * message,
            uint16 length
    ) -> sint64;
};


#include "thread/ClientThread.hpp"

#include "impl/Client.hpp"

#endif //CONCURRENT_SV_CLIENT_HPP
