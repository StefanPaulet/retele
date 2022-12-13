//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include <list>
#include <map>
#include <mutex>


class Client {

private:
    struct PingingThreadParameter {
        int requestNr { 0 };
        int frequency { 0 };
        Client * client { nullptr };
    };

friend auto _pinging_main ( void * param ) -> void *; /* NOLINT(bugprone-reserved-identifier) */

private:
    std :: mutex _socket_lock;

private:
    sockaddr_in _server_info { };

private:
     int _server_fd { 0 };

private:
    pthread_t _writer_thread { };

private:
    pthread_t _pinging_threads [ 3 ] { };

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
