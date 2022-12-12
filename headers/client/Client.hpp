//
// Created by stefan on 24.11.2022.
//

#ifndef CONCURRENT_SV_CLIENT_HPP
#define CONCURRENT_SV_CLIENT_HPP

#include <list>
#include <map>

#include "thread/ClientThread.hpp"

class Client {

private:
    sockaddr_in _server_info { };

private:
     int _server_fd { 0 };

private:
    pthread_t _writerThread { };

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
    auto client_main () const -> void;
};

#include "impl/Client.hpp"

#endif //CONCURRENT_SV_CLIENT_HPP
